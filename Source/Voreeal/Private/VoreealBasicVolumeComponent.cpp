#include "VoreealPrivatePCH.h"
#include "VoreealBasicVolumeComponent.h"
#include "VoreealBlueprintLibrary.h"

#include "MessageLog.h"

UBasicVolumeComponent::UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_octree(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

	Volume = ObjectInitializer.CreateDefaultSubobject<UBasicVolume>(this, TEXT("NewVolume"));
	check(Volume);
}

UBasicVolumeComponent::~UBasicVolumeComponent()
{
	if (m_octree != nullptr)
	{
		delete m_octree;
	}
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

FString UBasicVolumeComponent::GetDetailedInfoInternal() const
{
	return (Volume != NULL) ? Volume->GetPathName(NULL) : TEXT("No_Volume");
}

void UBasicVolumeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_octree)
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

				FVoreealExtractorOptions Options(node->m_selfId, node->m_bounds, 0);

				AddTask(Volume, Options);

				//UE_LOG(LogTemp, Warning, TEXT("Voreeal: Queue Task!"));
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

				//UE_LOG(LogTemp, Warning, TEXT("Voreeal: Finish Task!"));
			}
		}
	}
}

bool UBasicVolumeComponent::SetBasicVolume(UBasicVolume* NewVolume)
{
	if (NewVolume == Volume)
		return false;

	AActor* Owner = GetOwner();
	if (!AreDynamicDataChangesAllowed() && Owner != NULL)
	{
		FMessageLog("PIE").Warning(FText::Format(
			FText::FromString(TEXT("Calling SetBasicVolume on '{0}' but Mobility is Static.")),
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

	EnsureRendering();
	// TODO: Mark dirty

	return true;
}

void UBasicVolumeComponent::DrawDebugOctree(const FLinearColor& Color, float Duration, float Thickness)
{
	if (m_octree)
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

void UBasicVolumeComponent::EnsureRendering()
{
	if (Volume != nullptr && m_octree == nullptr)
	{
		m_octree = new FSparseOctree(Volume, this, EOctreeConstructionModes::BoundCells);
	}
}
