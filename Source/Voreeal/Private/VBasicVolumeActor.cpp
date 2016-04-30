#include "VoreealPrivatePCH.h"
#include "VBasicVolumeActor.h"

#include "MessageLog.h"
#include "UObjectToken.h"
#include "MapErrors.h"

ABasicVolumeActor::ABasicVolumeActor(const class FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	BasicVolumeComponent = CreateDefaultSubobject<UBasicVolumeComponent>(TEXT("BasicVolumeComponent"));
	//BasicVolumeComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	BasicVolumeComponent->Mobility = EComponentMobility::Static;
	//BasicVolumeComponent->bGenerateOverlapEvents = false;

	RootComponent = BasicVolumeComponent;
}

void ABasicVolumeActor::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	if (!BasicVolumeComponent)
	{
		MapCheck.Warning()
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(FText::FromString(TEXT("Basic volume actor has NULL BasicVolumeComponent property - please delete"))))
			->AddToken(FMapErrorToken::Create(TEXT("BasicVolumeComponentNull")));
	}
	else if (BasicVolumeComponent->Volume == NULL)
	{
		MapCheck.Warning()
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(FText::FromString(TEXT("Basic volume actor has NULL BasicVolume property"))))
			->AddToken(FMapErrorToken::Create(TEXT("BasicVolumeNull")));
	}
}

bool ABasicVolumeActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);
	if (BasicVolumeComponent && BasicVolumeComponent->Volume)
	{
		Objects.Add(BasicVolumeComponent->Volume);
	}
	return true;
}

FString ABasicVolumeActor::GetDetailedInfoInternal() const
{
	return BasicVolumeComponent ? BasicVolumeComponent->GetDetailedInfoInternal() : TEXT("No_BasicVolumeComponent");
}

UBasicVolumeComponent* ABasicVolumeActor::GetBasicVolumeComponent() const
{
	return BasicVolumeComponent;
}
