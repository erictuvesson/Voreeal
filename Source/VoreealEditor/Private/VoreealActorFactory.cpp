#include "VoreealEditorPrivatePCH.h"
#include "VoreealActorFactory.h"

#include "VoreealBasicVolumeActor.h"
#include "VoreealEditorSettings.h"

#define LOCTEXT_NAMESPACE "Voreeal"

UVoreealActorFactory::UVoreealActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("Voreeal", "VoreealFactoryDisplayName", "Add Voxel Volume");
	NewActorClass = ABasicVolumeActor::StaticClass();
}

void UVoreealActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	if (UBasicVolume* VoxelVolume = Cast<UBasicVolume>(Asset))
	{
		auto* TypedActor = CastChecked<ABasicVolumeActor>(NewActor);
		TypedActor->GetBasicVolumeComponent()->SetBasicVolume(VoxelVolume);

		const UVoreealEditorSettings* VoreealSettings = GetDefault<UVoreealEditorSettings>();
		TypedActor->SetActorScale3D(VoreealSettings->DefaultScale);
	}
}

void UVoreealActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (UBasicVolume* VoxelVolume = Cast<UBasicVolume>(Asset))
	{
		if (ABasicVolumeActor* TypedActor = Cast<ABasicVolumeActor>(CDO))
		{
			TypedActor->GetBasicVolumeComponent()->SetBasicVolume(VoxelVolume);
		}
	}
}

bool UVoreealActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
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