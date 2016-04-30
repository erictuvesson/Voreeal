#include "VoreealPrivatePCH.h"
#include "VRegionVolumeComponent.h"

#include "MessageLog.h"

URegionVolumeComponent::URegionVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	//SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}

void URegionVolumeComponent::PostLoad()
{
	Super::PostLoad();

	if (Volume)
	{
		Volume->ConditionalPostLoad();
	}
}

FString URegionVolumeComponent::GetDetailedInfoInternal() const
{
	return (Volume != NULL) ? Volume->GetPathName(NULL) : TEXT("No_Volume");
}

bool URegionVolumeComponent::SetVolume(UBaseVolume* NewVolume)
{
	if (NewVolume == Volume)
	{
		return false;
	}

	AActor* Owner = GetOwner();
	if (!AreDynamicDataChangesAllowed() && Owner != NULL)
	{
		FMessageLog("PIE").Warning(FText::Format(
			FText::FromString(TEXT("Calling SetVolume on '{0}' but Mobility is Static.")),
			FText::FromString(GetPathName())));
		return false;
	}

	Volume = NewVolume;

	// TODO: Mark dirty

	return true;
}
