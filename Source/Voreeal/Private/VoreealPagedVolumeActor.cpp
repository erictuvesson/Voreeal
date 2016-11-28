#include "VoreealPrivatePCH.h"
#include "VoreealPagedVolumeActor.h"

#include "MessageLog.h"
#include "UObjectToken.h"
#include "MapErrors.h"

APagedVolumeActor::APagedVolumeActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PagedVolumeComponent = CreateDefaultSubobject<UPagedVolumeComponent>(TEXT("PagedVolumeComponent"));

	RootComponent = PagedVolumeComponent;
}

void APagedVolumeActor::CheckForErrors()
{
	Super::CheckForErrors();

	FMessageLog MapCheck("MapCheck");

	if (!PagedVolumeComponent)
	{
		MapCheck.Warning()
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(FText::FromString(TEXT("Paged volume actor has NULL PagedVolumeComponent property - please delete"))))
			->AddToken(FMapErrorToken::Create(TEXT("PagedVolumeComponentNull")));
	}
	else if (PagedVolumeComponent->Volume == NULL)
	{
		MapCheck.Warning()
			->AddToken(FUObjectToken::Create(this))
			->AddToken(FTextToken::Create(FText::FromString(TEXT("Paged volume actor has NULL PagedVolume property"))))
			->AddToken(FMapErrorToken::Create(TEXT("PagedVolumeNull")));
	}
}

bool APagedVolumeActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);
	if (PagedVolumeComponent && PagedVolumeComponent->Volume)
	{
		Objects.Add(PagedVolumeComponent->Volume);
	}
	return true;
}

FString APagedVolumeActor::GetDetailedInfoInternal() const
{
	return PagedVolumeComponent ? PagedVolumeComponent->GetDetailedInfoInternal() : TEXT("No_PagedVolumeComponent");
}

UPagedVolumeComponent* APagedVolumeActor::GetPagedVolumeComponent() const
{
	return PagedVolumeComponent;
}
