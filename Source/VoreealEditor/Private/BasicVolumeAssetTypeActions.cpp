#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeAssetTypeActions.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"

#include "BasicVolumeEditor/BasicVolumeEditor.h"
#include "VoreealBasicVolume.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FBasicVolumeAssetTypeActions::FBasicVolumeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FBasicVolumeAssetTypeActions::GetName() const
{
	return LOCTEXT("FBasicVolumeAssetTypeActionsName", "Basic Volume");
}

FColor FBasicVolumeAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FBasicVolumeAssetTypeActions::GetSupportedClass() const
{
	return UBasicVolume::StaticClass();
}

void FBasicVolumeAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UBasicVolume* Volume = Cast<UBasicVolume>(*ObjIt))
		{
			TSharedRef<FBasicVolumeEditor> NewSpriteEditor(new FBasicVolumeEditor());
			NewSpriteEditor->InitEditor(Mode, EditWithinLevelEditor, Volume);
		}
	}
}

uint32 FBasicVolumeAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

void FBasicVolumeAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

#undef LOCTEXT_NAMESPACE