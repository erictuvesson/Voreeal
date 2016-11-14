#pragma once

#include "AssetTypeActions_Base.h"

class UBasicVolume;

class FBasicVolumeAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FBasicVolumeAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	// Begin IAssetTypeActions Interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
	// End IAssetTypeActions Interface

private:
	EAssetTypeCategories::Type MyAssetCategory;
};
