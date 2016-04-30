#pragma once

#include <ThumbnailHelpers.h>

#include "VBasicVolume.h"
#include "VBasicVolumeActor.h"

#include "VVolumeThumbnailRenderer.generated.h"

class VOREEALEDITOR_API FVVolumeThumbnailScene : public FThumbnailPreviewScene
{
public:
	FVVolumeThumbnailScene();

	void SetVolume(UBasicVolume* Volume);

protected:

	// FThumbnailPreviewScene interface
	virtual void GetViewMatrixParameters(const float InFOVDegrees, FVector& OutOrigin, float& OutOrbitPitch, float& OutOrbitYaw, float& OutOrbitZoom) const override;
	// End of FThumbnailPreviewScene implementation

private:
	ABasicVolumeActor* PreviewActor;
};

/**
 *
 * For this to work you have to change the config.
 * This should be added to DefaultEditor.ini
 * 
 *   [/Script/UnrealEd.ThumbnailManager]
 *   +RenderableThumbnailTypes=(ClassNeedingThumbnailName="/Script/Voreeal.VoreealVolume",RendererClassName="/Script/VoreealEditor.VoreealVolumeThumbnailRenderer")
 * 
 */
UCLASS(Config = Editor)
class VOREEALEDITOR_API UVVolumeThumbnailRenderer : public UDefaultSizedThumbnailRenderer
{
	GENERATED_BODY()

public:
	UVVolumeThumbnailRenderer(const class FObjectInitializer& ObjectInitializer);

	// UThumbnailRenderer interface
	virtual bool CanVisualizeAsset(UObject* Object) override;
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas) override;
	// End of UThumbnailRenderer interface

	// UObject implementation
	virtual void BeginDestroy() override;
	// End of UObject implementation

private:
	FVVolumeThumbnailScene* ThumbnailScene;
};
