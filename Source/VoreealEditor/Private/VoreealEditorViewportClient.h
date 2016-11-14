#pragma once

#include "SceneViewport.h"
#include "AssetEditorModeManager.h"

class FVoreealEditorViewportClient : public FEditorViewportClient
{
public:
	/** Constructor */
	explicit FVoreealEditorViewportClient(const TWeakPtr<class SEditorViewport>& InEditorViewportWidget = nullptr);
	~FVoreealEditorViewportClient();

	// Begin FViewportClient interface
	virtual void Tick(float DeltaSeconds) override;
	// End FViewportClient interface

	// Begin FEditorViewportClient Interface
	virtual FLinearColor GetBackgroundColor() const override;
	// End FEditorViewportClient Interface

	// Begin FSerializableObject Interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End FSerializableObject Interface

protected:
	virtual FBox GetDesiredFocusBounds() const
	{
		return FBox(ForceInitToZero);
	}

};
