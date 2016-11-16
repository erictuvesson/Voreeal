
#pragma once

#include "VoreealBasicVolumeThumbnailRenderer.generated.h"

UCLASS()
class UBasicVolumeThumbnailRenderer : public UDefaultSizedThumbnailRenderer
{
	GENERATED_BODY()

public:
	// Begin UThumbnailRenderer Interface
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas) override;
	// End UThumbnailRenderer Interface
};
