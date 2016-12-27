#include "VoreealPrivatePCH.h"
#include "VoreealBasicVolumeComponent.h"
#include "VoreealBlueprintLibrary.h"

#include "MessageLog.h"

UBasicVolumeComponent::UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_octree(nullptr)
{
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	Volume = ObjectInitializer.CreateDefaultSubobject<UBasicVolume>(this, TEXT("NewVolume"));

	EnsureRendering();
}

void UBasicVolumeComponent::PostLoad()
{
	Super::PostLoad();

	if (Volume)
	{
		Volume->ConditionalPostLoad();
		EnsureRendering();
	}
}

void UBasicVolumeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// MarkVolumeDirty();
}

FString UBasicVolumeComponent::GetDetailedInfoInternal() const
{
	return (Volume != NULL) ? Volume->GetPathName(NULL) : TEXT("No_Volume");
}

void UBasicVolumeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_octree.IsValid())
	{
		// Traverse all the nodes and see if they need to be updated,
		// if so we queue a task.
		m_octree->Traverse([=](FSparseOctreeNode* node) -> ETraverseOptions
		{
			if (node->m_hasChildren)
			{
				return ETraverseOptions::Continue;
			}

			if (!node->IsUpToDate() && !node->IsSceduled() && !node->IsTaskRunning())
			{
				node->m_lastSceduledForUpdate = FTimespan(0, 0, FPlatformTime::Seconds());
				node->m_bTaskRunning = true;

				FVoreealExtractorOptions Options(TWeakPtr<FSparseOctree>(m_octree), node->m_selfId, node->m_bounds, 0);

				SCOPE_CYCLE_COUNTER(STAT_RequestMesh);

				AddTask(Volume, Options);
			}

			return ETraverseOptions::Continue;
		});

		// Get Finished Tasks
		TSharedPtr<FVoreealMesh> Task;
		while (FindFinishedTask(Task))
		{
			if (Task.IsValid())
			{
				FVoreealMesh* Mesh = Task.Get();

				Mesh->CreateSection(MeshComponent, true);

				FSparseOctreeNode* Node = m_octree->GetNodeAt(Mesh->GetOptions().Identifier);
				Node->m_meshLastChanged = FTimespan(0, 0, FPlatformTime::Seconds());
				Node->m_bTaskRunning = false;

				SCOPE_CYCLE_COUNTER(STAT_ReturnMesh);
			}
		}
	}
}

void UBasicVolumeComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Bind somewhere else??
	Volume->OnChanged.AddDynamic(this, &UBasicVolumeComponent::OnVolumeChanged);
}

bool UBasicVolumeComponent::SetBasicVolume(UBasicVolume* NewVolume)
{
	if (NewVolume == Volume && NewVolume == nullptr)
		return false;

	AActor* Owner = GetOwner();
	if (!AreDynamicDataChangesAllowed() && Owner != NULL)
	{
		FMessageLog("PIE").Warning(FText::Format(
			FText::FromString(TEXT("Calling SetBasicVolume on '{0}' but Mobility is Static.")),
			FText::FromString(GetPathName())));
		return false;
	}

	if (m_octree.IsValid())
	{
		m_octree.Reset();
	}

	Volume = NewVolume;

	// If there were a volume before we call then we force gc
	UWorld* World = GetWorld();
	if (World)
	{
		World->ForceGarbageCollection(true);
	}

	EnsureRendering();

	return true;
}

void UBasicVolumeComponent::DrawDebugOctree(const FLinearColor& Color, float Duration, float Thickness)
{
	if (m_octree.IsValid())
	{
		m_octree->Traverse([=](FSparseOctreeNode* node) -> ETraverseOptions
		{
			if (node->m_hasChildren)
			{
				return ETraverseOptions::Continue;
			}

			UVoreealBlueprintLibrary::DrawDebugRegion(this, this->GetComponentTransform(), 
				node->m_bounds, Color.ToFColor(true), Duration, Thickness);

			return ETraverseOptions::Skip;
		});
	}
}

void UBasicVolumeComponent::MarkVolumeDirty()
{
	// Rebuild mesh
	if (m_octree.IsValid())
	{
		m_octree->MarkChange(Volume->GetEnclosingRegion(), FTimespan(0, 0, FPlatformTime::Seconds()));
	}
}

bool UBasicVolumeComponent::PickFirstSolidVoxel(const FVector& Start, const FVector& End, FIntVector& HitPoint) const
{
	if (Volume == nullptr)
	{
		return false;
	}

	FVector NewStart;
	FVector NewEnd;

	WorldPositionToVolumePosition(Start, NewStart);
	WorldPositionToVolumePosition(End, NewEnd);

	return Volume->PickFirstSolidVoxel(NewStart, NewEnd, HitPoint);
}

bool UBasicVolumeComponent::PickLastSolidVoxel(const FVector& Start, const FVector& End, FIntVector& HitPoint) const
{
	if (Volume == nullptr)
	{
		return false;
	}

	FVector NewStart;
	FVector NewEnd;

	WorldPositionToVolumePosition(Start, NewStart);
	WorldPositionToVolumePosition(End, NewEnd);

	return Volume->PickLastSolidVoxel(NewStart, NewEnd, HitPoint);
}

void UBasicVolumeComponent::OnVolumeChanged(FVoreealRegion Region)
{
	m_octree->MarkChangeNow(Region);
}

void UBasicVolumeComponent::EnsureRendering()
{
	if (Volume != nullptr && !m_octree.IsValid())
	{
		m_octree = MakeShareable(new FSparseOctree(Volume, this, EOctreeConstructionModes::BoundCells));
	}
}
