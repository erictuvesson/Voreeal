#include "VoreealEditorPrivatePCH.h"
#include "VActorFactory.h"

#include "VBasicVolumeActor.h"

#define LOCTEXT_NAMESPACE "Voreeal"

UVActorFactory::UVActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("Voreeal", "VoreealFactoryDisplayName", "Add Voxel Volume");
	NewActorClass = ABasicVolumeActor::StaticClass();
}

void UVActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	if (UBasicVolume* VoxelVolume = Cast<UBasicVolume>(Asset))
	{
		auto* TypedActor = CastChecked<ABasicVolumeActor>(NewActor);
		TypedActor->GetBasicVolumeComponent()->SetBasicVolume(VoxelVolume);
	}
}

void UVActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (UBasicVolume* VoxelVolume = Cast<UBasicVolume>(Asset))
	{
		if (ABasicVolumeActor* TypedActor = Cast<ABasicVolumeActor>(CDO))
		{
			TypedActor->GetBasicVolumeComponent()->SetBasicVolume(VoxelVolume);
		}
	}
}

bool UVActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid() && AssetData.GetClass()->IsChildOf(UBasicVolume::StaticClass()))
	{
		return true;
	}
	else
	{
		OutErrorMsg = NSLOCTEXT("Voreeal", "CanCreateActorFrom_NoVoxelVolume", "No voxel volume was specified.");
		return false;
	}
}

#undef LOCTEXT_NAMESPACE