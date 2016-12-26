#pragma once

#include "VoreealBasicVolumeComponent.h"
#include "VoreealEditorViewportClient.h"

#include "PaletteEditor.h"
#include "PaletteEditorViewportClient.h"

#include "SSingleObjectDetailsPanel.h"
#include "SceneViewport.h"
#include "PreviewScene.h"
#include "ScopedTransaction.h"
#include "AssetData.h"

class FPaletteEditorViewportClient : public FVoreealEditorViewportClient
{
public:
	// Constructor 
	FPaletteEditorViewportClient(TWeakPtr<FPaletteEditor> InEditor, TWeakPtr<class SEditorViewport> InEditorViewportPtr);

	// Begin FViewportClient Interface
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas) override;
	virtual void Tick(float DeltaSeconds) override;
	// End FViewportClient Interface

	// Begin FEditorViewportClient Interface
	virtual void ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;
	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad) override;
	virtual void TrackingStarted(const struct FInputEventState& InInputState, bool bIsDragging, bool bNudge) override;
	virtual void TrackingStopped() override;
	virtual FLinearColor GetBackgroundColor() const override;
	// End FEditorViewportClient Interface

	// Invalidate any references to the palette being edited; it has changed
	void NotifyPaletteBeingEditedHasChanged();

protected:
	// Begin FVoreealEditorViewportClient Interface
	virtual FBox GetDesiredFocusBounds() const override;
	// End FVoreealEditorViewportClient Interface

private:
	// The preview scene
	FPreviewScene OwnedPreviewScene;

	// Sprite editor that owns this viewport
	TWeakPtr<FPaletteEditor> EditorPtr;

	// Are we currently manipulating something?
	bool bManipulating;

	// Did we dirty something during manipulation?
	bool bManipulationDirtiedSomething;

	// Pointer back to the editor viewport control that owns us
	TWeakPtr<class SEditorViewport> EditorViewportPtr;

	// The current transaction for undo/redo
	class FScopedTransaction* ScopedTransaction;

private:
	UVoxelPalette* GetPaletteBeingEdited() const { return EditorPtr.Pin()->GetPaletteBeingEdited(); }
};
