#include "VoreealPrivatePCH.h"
#include "VRegionVolumeActor.h"

#include "MessageLog.h"
#include "UObjectToken.h"
#include "MapErrors.h"

ARegionVolumeActor::ARegionVolumeActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ARegionVolumeActor::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	if (!RegionVolumeComponent)
	{
		MapCheck.Warning()
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(FText::FromString(TEXT("Region volume actor has NULL RegionVolumeComponent property - please delete"))))
			->AddToken(FMapErrorToken::Create(TEXT("RegionVolumeComponentNull")));
	}
	else if (RegionVolumeComponent->Volume == NULL)
	{
		MapCheck.Warning()
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(FText::FromString(TEXT("Region volume actor has NULL Volume property"))))
			->AddToken(FMapErrorToken::Create(TEXT("VolumeNull")));
	}
}

bool ARegionVolumeActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);
	if (RegionVolumeComponent && RegionVolumeComponent->Volume)
	{
		Objects.Add(RegionVolumeComponent->Volume);
	}
	return true;
}

FString ARegionVolumeActor::GetDetailedInfoInternal() const
{
	return RegionVolumeComponent ? RegionVolumeComponent->GetDetailedInfoInternal() : TEXT("No_BasicVolumeComponent");
}

URegionVolumeComponent* ARegionVolumeActor::GetRegionVolumeComponent() const
{
	return RegionVolumeComponent;
}
