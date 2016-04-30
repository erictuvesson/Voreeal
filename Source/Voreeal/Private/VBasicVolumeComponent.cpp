#include "VoreealPrivatePCH.h"
#include "VBasicVolumeComponent.h"

#include "MessageLog.h"

UBasicVolumeComponent::UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	//SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}

void UBasicVolumeComponent::PostLoad()
{
	Super::PostLoad();

	if (Volume)
	{
		Volume->ConditionalPostLoad();
	}
}

FString UBasicVolumeComponent::GetDetailedInfoInternal() const
{
	return (Volume != NULL) ? Volume->GetPathName(NULL) : TEXT("No_Volume");
}

bool UBasicVolumeComponent::SetBasicVolume(class UBasicVolume* NewVolume)
{
	if (NewVolume == Volume)
	{
		return false;
	}

	AActor* Owner = GetOwner();
	if (!AreDynamicDataChangesAllowed() && Owner != NULL)
	{
		FMessageLog("PIE").Warning(FText::Format(
			FText::FromString(TEXT("Calling SetBasicVolume on '{0}' but Mobility is Static.")),
			FText::FromString(GetPathName())));
		return false;
	}

	Volume = NewVolume;

	 // TODO: Mark dirty

	return true;
}


/*

Async<int>(EAsyncExecution::ThreadPool, [this]
{
	FProceduralMesh Result;

	this->Volume->ExtractMesh(Volume->GetEnclosingRegion(), 0, Result);

	// TODO: Is procedural mesh thread safe?
	TGraphTask<FMeshUpdateTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(MeshComponent, Result);

	return 0;
});

*/
