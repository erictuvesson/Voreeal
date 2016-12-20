#include "VoreealPrivatePCH.h"
#include "VoreealPagedVolumeComponent.h"
#include "VoreealBlueprintLibrary.h"
#include <PolyVox/Impl/Morton.h>

#include "MessageLog.h"

FPagedVolumeChunk::FPagedVolumeChunk(TSharedPtr<FSparseOctree> Octree, uint32 LocationHash, FVoreealRegion Bounds)
	: m_octree(Octree)
	, m_locationHash(LocationHash)
	, m_bounds(Bounds)
{

}

UPagedVolumeComponent::UPagedVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	Volume = ObjectInitializer.CreateDefaultSubobject<UVoreealPagedVolume>(this, TEXT("NewPagedVolume"));
}

void UPagedVolumeComponent::PostLoad()
{
	Super::PostLoad();

	if (Volume)
	{
		Volume->ConditionalPostLoad();
	}
}

FString UPagedVolumeComponent::GetDetailedInfoInternal() const
{
	return (Volume != NULL) ? Volume->GetPathName(NULL) : TEXT("No_Volume");
}

void UPagedVolumeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check what chunks should be loaded.

	// TODO: 


	// Loop and create new tasks if required.
	for (TSharedPtr<FPagedVolumeChunk> Chunk : ArrayChunks)
	{
		if (Chunk->m_octree.IsValid())
		{
			// Traverse all the nodes and see if they need to be updated,
			// if so we queue a task.
			Chunk->m_octree->Traverse([=](FSparseOctreeNode* Node) -> ETraverseOptions
			{
				if (Node->m_hasChildren)
				{
					return ETraverseOptions::Continue;
				}

				if (!Node->IsUpToDate() && !Node->IsSceduled() && !Node->IsTaskRunning())
				{
					Node->m_lastSceduledForUpdate = FTimespan(0, 0, FPlatformTime::Seconds());
					Node->m_bTaskRunning = true;

					FVoreealExtractorOptions Options(Chunk->m_octree, Node->m_selfId, Node->m_bounds, 0);

					AddTask(Volume, Options);
				}

				return ETraverseOptions::Continue;
			});
		}
	}

	// Get Finished Tasks
	TSharedPtr<FVoreealMesh> Task;
	while (FindFinishedTask(Task))
	{
		if (Task.IsValid())
		{
			// Get the task result
			FVoreealMesh* Mesh = Task.Get();
			FVoreealExtractorOptions Options = Mesh->GetOptions();

			const uint32 LocationHash = MortonHash(Options.PagedIdentifier.X, Options.PagedIdentifier.Y, Options.PagedIdentifier.Z);

			// Get the procedural mesh component 
			UProceduralMeshComponent* MComponent = nullptr;
			if (ProceduralMeshComponents.Contains(LocationHash))
				MComponent = ProceduralMeshComponents[LocationHash];

			if (MComponent == nullptr)
			{
				MComponent = NewObject<UProceduralMeshComponent>();
				ProceduralMeshComponents[LocationHash] = MComponent;
			}

			// Update the mesh node
			Mesh->CreateSection(MComponent, true);

			// this shouldn't be needed, but lets have it here in case.
			if (Options.Octree.IsValid())
			{
				TSharedPtr<FSparseOctree> Octree = Options.Octree.Pin();
				
				FSparseOctreeNode* Node = Octree->GetNodeAt(Options.Identifier);
				Node->m_meshLastChanged = FTimespan(0, 0, FPlatformTime::Seconds());
				Node->m_bTaskRunning = false;
			}
		}
	}
}

void UPagedVolumeComponent::BeginPlay()
{
	Super::BeginPlay();
	Volume->OnChanged.AddDynamic(this, &UPagedVolumeComponent::OnVolumeChanged);
}

bool UPagedVolumeComponent::SetPagedVolume(UVoreealPagedVolume* NewVolume)
{
	if (NewVolume == Volume && NewVolume == nullptr)
		return false;

	AActor* Owner = GetOwner();
	if (!AreDynamicDataChangesAllowed() && Owner != NULL)
	{
		FMessageLog("PIE").Warning(FText::Format(
			FText::FromString(TEXT("Calling SetPagedVolume on '{0}' but Mobility is Static.")),
			FText::FromString(GetPathName())));
		return false;
	}

	Volume = NewVolume;

	// If there were a volume before we call then we force gc
	UWorld* World = GetWorld();
	if (World)
	{
		World->ForceGarbageCollection(true);
	}

	return true;
}

FPagedVolumeChunk* UPagedVolumeComponent::GetChunk(int32 X, int32 Y, int32 Z) const
{
	const uint32 LocationHash = MortonHash(X, Y, Z);

	// check if it's the last one we accessed. 
	if (getChunkCache) //< TODO: Validate?
	{
		if (getChunkCache->m_locationHash == LocationHash)
		{
			return getChunkCache;
		}
	}

	// loop all chunks
	for (TSharedPtr<FPagedVolumeChunk> Chunk : ArrayChunks)
	{
		if (Chunk->m_octree.IsValid())
		{
			if (Chunk->m_locationHash == LocationHash)
			{
				return getChunkCache = Chunk.Get();
			}
		}
	}

	return nullptr;
}

FPagedVolumeChunk* UPagedVolumeComponent::CreateChunk(int32 X, int32 Y, int32 Z)
{
	// We assume it doesn't exists

	const uint32 LocationHash = MortonHash(X, Y, Z);

	FVoreealRegion Region = FVoreealRegion(X * 128, Y * 128, Z * 128, 128, 128, 128);

	TSharedPtr<FPagedVolumeChunk> Result = MakeShareable(new FPagedVolumeChunk(MakeShareable(new FSparseOctree(Volume, this, EOctreeConstructionModes::BoundCells)), LocationHash, Region));

	ArrayChunks.Add(Result);

	return Result.Get();
}

void UPagedVolumeComponent::OnVolumeChanged(FVoreealRegion Region)
{
	// Check if the last accessed chunk matches.
	if (onVolumeCache)
	{
		EContainmentType ContainResult = FVoreealRegion::Contains(onVolumeCache->m_bounds, Region);
		if (ContainResult == EContainmentType::Contains)
		{
			onVolumeCache->m_octree->MarkChangeNow(Region);

			// TODO: Update rest of the volume.
			return;
		}
	}

	//maximum chunks intersecting this region, 
	//	assuming non-coincident edges, and uniform chunk size (as created above):
	uint32 i = 0;
	uint32 const iCrit = (Region.Width/128 +2) * (Region.Height/128 +2) * (Region.Depth/128 +2);

	// loop all chunks
	for (TSharedPtr<FPagedVolumeChunk> Chunk : ArrayChunks)
	{
		if (Chunk->m_octree.IsValid())
		{
			EContainmentType ContainResult = FVoreealRegion::Contains(onVolumeCache->m_bounds, Region);
			if (ContainResult == EContainmentType::Contains)
			{
				onVolumeCache = Chunk.Get();
				onVolumeCache->m_octree->MarkChangeNow(Region);
				return;
			}				
			if (ContainResult == EContainmentType::Intersects)
			{
				onVolumeCache = Chunk.Get();
				onVolumeCache->m_octree->MarkChangeNow(Region);
				if(++i >= iCrit) return;
			}
		}
	}
}

uint32 UPagedVolumeComponent::MortonHash(int32 X, int32 Y, int32 Z)
{
	return (PolyVox::morton256_x[X & 0xff] | PolyVox::morton256_y[Y & 0xff] | PolyVox::morton256_z[Z & 0xff]);
}
