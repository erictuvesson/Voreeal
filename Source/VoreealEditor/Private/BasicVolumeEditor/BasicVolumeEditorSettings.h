#pragma once

#include "BasicVolumeEditorSettings.generated.h"

// Settings for Voreeal's basic volume editor
UCLASS(config=EditorPerProjectUserSettings)
class UBasicVolumeEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	UBasicVolumeEditorSettings();

	// Should the grid be shown by default when the editor is opened?
	UPROPERTY(config, EditAnywhere, Category=Background)
	bool bShowGridByDefault;
};
