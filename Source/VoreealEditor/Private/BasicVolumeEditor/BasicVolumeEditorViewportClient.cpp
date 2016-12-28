#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeEditorViewportClient.h"

#include "SceneViewport.h"
#include "PreviewScene.h"
#include "ScopedTransaction.h"
#include "Runtime/Engine/Public/ComponentReregisterContext.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "AssetRegistryModule.h"
#include "CanvasTypes.h"
#include "CanvasItem.h"

#include "SEditorViewport.h"
#include "SNotificationList.h"
#include "NotificationManager.h"

#include "BasicVolumeEditorSettings.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

FBasicVolumeEditorViewportClient::FBasicVolumeEditorViewportClient(TWeakPtr<FBasicVolumeEditor> InEditor, TWeakPtr<class SEditorViewport> InEditorViewportPtr)
	: CurrentMode(EBasicVolumeEditorMode::ViewMode)
	, EditorPtr(InEditor)
	, EditorViewportPtr(InEditorViewportPtr)
{
	check(EditorPtr.IsValid() && EditorViewportPtr.IsValid());

	Widget->SetUsesEditorModeTools(ModeTools);

	PreviewScene = &OwnedPreviewScene;
	((FAssetEditorModeManager*)ModeTools)->SetPreviewScene(PreviewScene);

	SetRealtime(true);

	bManipulating = false;
	bManipulationDirtiedSomething = false;
	ScopedTransaction = nullptr;

	bShowOctree = true;
	bShowSockets = true;
	bShowPivot = true;

	DrawHelper.bDrawGrid = GetDefault<UBasicVolumeEditorSettings>()->bShowGridByDefault;

	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.SetCompositeEditorPrimitives(true);

	// Create a render component for the volume being edited
	{
		RenderVolumeComponent = NewObject<UBasicVolumeComponent>();
		UBasicVolume* Volume = GetVolumeBeingEdited();
		RenderVolumeComponent->SetBasicVolume(Volume);

		PreviewScene->AddComponent(RenderVolumeComponent, FTransform::Identity);
	}
}

void FBasicVolumeEditorViewportClient::ActivateEditMode()
{
	ModeTools->SetWidgetMode(FWidget::WM_Translate);
}

#pragma region // Begin Show Flags
void FBasicVolumeEditorViewportClient::SetShowOctree(bool bShow)
{
	bShowOctree = !bShowOctree;
	Invalidate(); 
}

void FBasicVolumeEditorViewportClient::ToggleShowOctree()
{
	bShowOctree = !bShowOctree;
	Invalidate();
}

bool FBasicVolumeEditorViewportClient::IsSetShowOctreeChecked() const
{
	return bShowOctree;
}

void FBasicVolumeEditorViewportClient::ToggleShowSockets()
{
	bShowSockets = !bShowSockets;
	Invalidate();
}

bool FBasicVolumeEditorViewportClient::IsShowSocketsChecked() const
{ 
	return bShowSockets; 
}

void FBasicVolumeEditorViewportClient::ToggleShowPivot()
{ 
	bShowPivot = !bShowPivot; 
	Invalidate(); 
}

bool FBasicVolumeEditorViewportClient::IsShowPivotChecked() const
{ 
	return bShowPivot; 
}
#pragma endregion

void FBasicVolumeEditorViewportClient::EnterViewMode()
{ 
	InternalActivateNewMode(EBasicVolumeEditorMode::ViewMode); 
}

void FBasicVolumeEditorViewportClient::EnterEditMode()
{ 
	InternalActivateNewMode(EBasicVolumeEditorMode::EditMode); 
}

bool FBasicVolumeEditorViewportClient::IsInViewMode() const
{ 
	return CurrentMode == EBasicVolumeEditorMode::ViewMode; 
}

bool FBasicVolumeEditorViewportClient::IsInEditMode() const
{ 
	return CurrentMode == EBasicVolumeEditorMode::EditMode; 
}

void FBasicVolumeEditorViewportClient::DrawCanvas(FViewport& pViewport, FSceneView& View, FCanvas& Canvas)
{
	const bool bIsHitTesting = Canvas.IsHitTesting();
	if (!bIsHitTesting)
	{
		Canvas.SetHitProxy(nullptr);
	}

	if (!EditorPtr.IsValid())
	{
		return;
	}

	UBasicVolume* Volume = GetVolumeBeingEdited();

	switch (CurrentMode)
	{
	default:
	case EBasicVolumeEditorMode::ViewMode:
		break;
	case EBasicVolumeEditorMode::EditMode:
		break;
	}

	if (bShowSockets && !IsInEditMode())
	{
		//FSpriteGeometryEditMode* GeometryEditMode = ModeTools->GetActiveModeTyped<FSpriteGeometryEditMode>(FSpriteGeometryEditMode::EM_SpriteGeometry);
		//FSocketEditingHelper::DrawSocketNames(GeometryEditMode, RenderVolumeComponent, Viewport, View, Canvas);
	}

	FEditorViewportClient::DrawCanvas(pViewport, View, Canvas);
}

void FBasicVolumeEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);

	if (IsInEditMode())
	{
		if (bShowPivot)
		{
			const bool bCanSelectPivot = false;
			const bool bHitTestingForPivot = PDI->IsHitTesting() && bCanSelectPivot;
			FUnrealEdUtils::DrawWidget(View, PDI, RenderVolumeComponent->ComponentToWorld.ToMatrixWithScale(), 0, 0, EAxisList::XZ, EWidgetMovementMode::WMM_Translate, bHitTestingForPivot);
		}

		if (bShowSockets)
		{
			//FSpriteGeometryEditMode* GeometryEditMode = ModeTools->GetActiveModeTyped<FSpriteGeometryEditMode>(FSpriteGeometryEditMode::EM_SpriteGeometry);
			//FSocketEditingHelper::DrawSockets(GeometryEditMode, RenderSpriteComponent, View, PDI);
		}
	}
}

FBox FBasicVolumeEditorViewportClient::GetDesiredFocusBounds() const
{
	UBasicVolumeComponent* ComponentToFocusOn = RenderVolumeComponent;
	return ComponentToFocusOn->Bounds.GetBox();
}

void FBasicVolumeEditorViewportClient::Tick(float DeltaSeconds)
{
	if (UBasicVolume* Volume = GetVolumeBeingEdited())
	{

	}

	FVoreealEditorViewportClient::Tick(DeltaSeconds);

	// Draw the octree
	if (bShowOctree && RenderVolumeComponent)
	{
		RenderVolumeComponent->DrawDebugOctree(FLinearColor::Red, 0, 1.0f);
	}

	if (!GIntraFrameDebuggingGameThread)
	{
		OwnedPreviewScene.GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

void FBasicVolumeEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	const FViewportClick Click(&View, this, Key, Event, HitX, HitY);
	const bool bIsCtrlKeyDown = Viewport->KeyState(EKeys::LeftControl) || Viewport->KeyState(EKeys::RightControl);
	const bool bIsShiftKeyDown = Viewport->KeyState(EKeys::LeftShift) || Viewport->KeyState(EKeys::RightShift);
	const bool bIsAltKeyDown = Viewport->KeyState(EKeys::LeftAlt) || Viewport->KeyState(EKeys::RightAlt);

	bool bHandled = false;
	if (!bHandled)
	{
		FVoreealEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);
	}
}

bool FBasicVolumeEditorViewportClient::InputKey(FViewport* pViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
	bool bHandled = false;
	FInputEventState InputState(pViewport, Key, Event);

	// Pass keys to standard controls, if we didn't consume input
	return (bHandled) ? true : FEditorViewportClient::InputKey(pViewport, ControllerId, Key, Event, AmountDepressed, bGamepad);
}

void FBasicVolumeEditorViewportClient::TrackingStarted(const struct FInputEventState& InInputState, bool bIsDragging, bool bNudge)
{
	// Begin transacting.  Give the current editor mode an opportunity to do the transacting.
	const bool bTrackingHandledExternally = ModeTools->StartTracking(this, Viewport);

	if (!bManipulating && bIsDragging && !bTrackingHandledExternally)
	{
		bManipulating = true;
		bManipulationDirtiedSomething = false;
	}
}

void FBasicVolumeEditorViewportClient::TrackingStopped()
{
	// Stop transacting.  Give the current editor mode an opportunity to do the transacting.
	const bool bTransactingHandledByEditorMode = ModeTools->EndTracking(this, Viewport);

	if (bManipulating && !bTransactingHandledByEditorMode)
	{
		bManipulating = false;
	}
}

FLinearColor FBasicVolumeEditorViewportClient::GetBackgroundColor() const
{
	return FLinearColor::Black;
}

void FBasicVolumeEditorViewportClient::NotifyVolumeBeingEditedHasChanged()
{
	// Refresh the viewport in case we were not in realtime mode
	Invalidate();

	// Update components to know about the new volume being edited
	UBasicVolume* Volume = GetVolumeBeingEdited();

	RenderVolumeComponent->SetBasicVolume(Volume);

	InternalActivateNewMode(CurrentMode);

	if (Volume != nullptr)
	{
		// Create and display a notification about the new volume being edited
		const FText NotificationErrorText = FText::Format(LOCTEXT("SwitchingToVolume", "Editing {0}"), FText::AsCultureInvariant(Volume->GetName()));
		FNotificationInfo Info(NotificationErrorText);
		Info.ExpireDuration = 2.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

void FBasicVolumeEditorViewportClient::InternalActivateNewMode(EBasicVolumeEditorMode NewMode)
{
	CurrentMode = NewMode;
	Viewport->InvalidateHitProxy();

	UBasicVolume* Volume = GetVolumeBeingEdited();

	switch (CurrentMode)
	{
	case EBasicVolumeEditorMode::ViewMode:
		break;
	case EBasicVolumeEditorMode::EditMode:
		break;
	}
}

#undef LOCTEXT_NAMESPACE
