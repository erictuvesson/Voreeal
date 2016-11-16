#pragma once

#include "VoreealBasicVolumeComponent.h"
#include "VoreealEditorViewportClient.h"

#include "BasicVolumeEditor.h"
#include "BasicVolumeEditorViewportClient.h"

#include "SSingleObjectDetailsPanel.h"
#include "SceneViewport.h"
#include "PreviewScene.h"
#include "ScopedTransaction.h"
#include "AssetData.h"

class FBasicVolumeEditorViewportClient : public FVoreealEditorViewportClient
{
public:
	// Constructor 
	FBasicVolumeEditorViewportClient(TWeakPtr<FBasicVolumeEditor> InEditor, TWeakPtr<class SEditorViewport> InEditorViewportPtr);

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

	// Activate the edit mode.
	void ActivateEditMode();

	// Sets the show octree flag.
	void SetShowOctree(bool bShow);

	// Callback for toggling the octree show flag.
	void ToggleShowOctree();

	// Callback for checking the octree show flag.
	bool IsSetShowOctreeChecked() const;

	// Callback for toggling the sockets show flag.
	void ToggleShowSockets();

	// Callback for checking the sockets show flag.
	bool IsShowSocketsChecked() const;

	// Callback for toggling the privot show flag.
	void ToggleShowPivot();

	// Callback for checking the privot show flag.
	bool IsShowPivotChecked() const;

	// Enter the view mode.
	void EnterViewMode();

	// Enter the edit mode.
	void EnterEditMode();

	// Callback for checking if the editor is in view mode.
	bool IsInViewMode() const;

	// Callback for checking if the editor is in edit mode.
	bool IsInEditMode() const;

	// Invalidate any references to the volume being edited; it has changed
	void NotifyVolumeBeingEditedHasChanged();

	EBasicVolumeEditorMode GetCurrentMode() const { return CurrentMode; }

protected:
	// Begin FVoreealEditorViewportClient Interface
	virtual FBox GetDesiredFocusBounds() const override;
	// End FVoreealEditorViewportClient Interface

private:
	// Editor mode
	EBasicVolumeEditorMode CurrentMode;

	// The preview scene
	FPreviewScene OwnedPreviewScene;

	// Sprite editor that owns this viewport
	TWeakPtr<FBasicVolumeEditor> EditorPtr;

	// Render component for the volume being edited
	UBasicVolumeComponent* RenderVolumeComponent;

	// Are we currently manipulating something?
	bool bManipulating;

	// Did we dirty something during manipulation?
	bool bManipulationDirtiedSomething;

	// Pointer back to the editor viewport control that owns us
	TWeakPtr<class SEditorViewport> EditorViewportPtr;

	// The current transaction for undo/redo
	class FScopedTransaction* ScopedTransaction;

	uint8 bShowOctree : 1;
	uint8 bShowSockets : 1;
	uint8 bShowPivot : 1;

private:
	UBasicVolume* GetVolumeBeingEdited() const { return EditorPtr.Pin()->GetVolumeBeingEdited(); }

	// TODO: Add raycast

	// Activates a new mode, clearing selection set, etc...
	void InternalActivateNewMode(EBasicVolumeEditorMode NewMode);
};
