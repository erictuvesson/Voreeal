#include "VoreealPrivatePCH.h"
#include "VoreealBasicVolumeComponent.h"
#include "VoreealBlueprintLibrary.h"

#include "MessageLog.h"

UBasicVolumeComponent::UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_octree(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;

	Volume = ObjectInitializer.CreateDefaultSubobject<UBasicVolume>(this, TEXT("NewVolume"));
	check(Volume);
}

UBasicVolumeComponent::~UBasicVolumeComponent()
{
	if (m_octree != nullptr)
	{
		delete m_octree;
	}

	if (timerHandle.IsValid())
	{
		//GetWorld()->GetTimerManager().ClearTimer(timerHandle);
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

void UBasicVolumeComponent::DrawDebugOctree(const FColor& Color, float Duration, float Thickness)
{
	if (m_octree)
	{
		m_octree->Traverse([=](FSparseOctreeNode* node) -> ETraverseOptions
		{
			if (node->m_hasChildren)
			{
				return ETraverseOptions::Continue;
			}

			UVoreealBlueprintLibrary::DrawDebugRegion(this, this->GetComponentTransform(), node->m_bounds, Color, Duration, Thickness);

			return ETraverseOptions::Skip;
		});
	}
}

void UBasicVolumeComponent::EnsureRendering()
{
	if (Volume != nullptr && m_octree == nullptr)
	{
		m_octree = new FSparseOctree(Volume, this, EOctreeConstructionModes::BoundCells);

		//UWorld* World = GetWorld();
		//FTimerManager& TimerManager = World->GetTimerManager();
		//TimerManager.SetTimer(timerHandle, this, &UBasicVolumeComponent::Update, 1.0f, true);
	}
}

void UBasicVolumeComponent::Update()
{
	if (m_octree)
	{
		//MeshComponent->CreateMeshSection();

		//m_octree->Update(FVector(0, 0, 0));
	}
}
