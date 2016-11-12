#include "VoreealPrivatePCH.h"
#include "VoreealProceduralBasicVolumeActor.h"

AProceduralBasicVolumeActor::AProceduralBasicVolumeActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BasicVolumeComponent = CreateDefaultSubobject<UBasicVolumeComponent>(TEXT("BasicVolumeComponent"));
	RootComponent = BasicVolumeComponent;
}

void AProceduralBasicVolumeActor::BeginPlay()
{
	Super::BeginPlay();

	if (BasicVolumeComponent->Volume)
	{
		//BasicVolumeComponent->Volume->Resize(FIntVector(256, 256, 256));

		BasicVolumeComponent->Volume->SetVoxelXYZ(0, 0, 0, FColor(255, 0, 0, 255));
	}
}

bool AProceduralBasicVolumeActor::GetReferencedContentObjects(TArray<UObject*>& Objects) const
{
	Super::GetReferencedContentObjects(Objects);
	if (BasicVolumeComponent && BasicVolumeComponent->Volume)
	{
		Objects.Add(BasicVolumeComponent->Volume);
	}
	return true;
}
