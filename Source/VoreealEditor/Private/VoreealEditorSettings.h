#pragma once

#include "VoreealEditorSettings.generated.h"

// Settings for the voreeal editor
UCLASS(config = EditorPerProjectUserSettings)
class UVoreealEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	UVoreealEditorSettings();

	// Background color in the voreeal editor.
	UPROPERTY(config, EditAnywhere, Category = Background, meta = (HideAlphaChannel))
	FColor BackgroundColor;

	// Should the grid be shown by default when the editor is opened? 
	UPROPERTY(config, EditAnywhere, Category = Background)
	bool bShowGridByDefault;

	// The default scale all the basic volume actors will spawn with.
	UPROPERTY(config, EditAnywhere, Category = LevelEditor)
	FVector DefaultScale;
};
