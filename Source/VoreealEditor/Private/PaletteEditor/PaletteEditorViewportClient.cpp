#include "VoreealEditorPrivatePCH.h"
#include "PaletteEditorViewportClient.h"

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

#include "PaletteEditorSettings.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

FPaletteEditorViewportClient::FPaletteEditorViewportClient(TWeakPtr<FPaletteEditor> InEditor, TWeakPtr<class SEditorViewport> InEditorViewportPtr)
	: EditorPtr(InEditor)
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

	EngineShowFlags.DisableAdvancedFeatures();
	EngineShowFlags.SetCompositeEditorPrimitives(true);
}

void FPaletteEditorViewportClient::DrawCanvas(FViewport& pViewport, FSceneView& View, FCanvas& Canvas)
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

	FEditorViewportClient::DrawCanvas(pViewport, View, Canvas);
}

void FPaletteEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);

	// ...
}

FBox FPaletteEditorViewportClient::GetDesiredFocusBounds() const
{
	return FBox();
}

void FPaletteEditorViewportClient::Tick(float DeltaSeconds)
{
	FVoreealEditorViewportClient::Tick(DeltaSeconds);

	if (!GIntraFrameDebuggingGameThread)
	{
		OwnedPreviewScene.GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

void FPaletteEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	const FViewportClick Click(&View, this, Key, Event, HitX, HitY);
	const bool bIsCtrlKeyDown = Viewport->KeyState(EKeys::LeftControl) || Viewport->KeyState(EKeys::RightControl);
	const bool bIsShiftKeyDown = Viewport->KeyState(EKeys::LeftShift) || Viewport->KeyState(EKeys::RightShift);
	const bool bIsAltKeyDown = Viewport->KeyState(EKeys::LeftAlt) || Viewport->KeyState(EKeys::RightAlt);
	bool bHandled = false;

	// ...

	if (!bHandled)
	{
		FVoreealEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);
	}
}

bool FPaletteEditorViewportClient::InputKey(FViewport* pViewport, int32 ControllerId, FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
	bool bHandled = false;
	FInputEventState InputState(pViewport, Key, Event);

	// Pass keys to standard controls, if we didn't consume input
	return (bHandled) ? true : FEditorViewportClient::InputKey(pViewport, ControllerId, Key, Event, AmountDepressed, bGamepad);
}

void FPaletteEditorViewportClient::TrackingStarted(const struct FInputEventState& InInputState, bool bIsDragging, bool bNudge)
{
	// Begin transacting.  Give the current editor mode an opportunity to do the transacting.
	const bool bTrackingHandledExternally = ModeTools->StartTracking(this, Viewport);

	if (!bManipulating && bIsDragging && !bTrackingHandledExternally)
	{
		bManipulating = true;
		bManipulationDirtiedSomething = false;
	}
}

void FPaletteEditorViewportClient::TrackingStopped()
{
	// Stop transacting.  Give the current editor mode an opportunity to do the transacting.
	const bool bTransactingHandledByEditorMode = ModeTools->EndTracking(this, Viewport);

	if (bManipulating && !bTransactingHandledByEditorMode)
	{
		bManipulating = false;
	}
}

FLinearColor FPaletteEditorViewportClient::GetBackgroundColor() const
{
	return FLinearColor::Black;
}

void FPaletteEditorViewportClient::NotifyPaletteBeingEditedHasChanged()
{
	// Refresh the viewport in case we were not in real time mode
	Invalidate();

	// Update components to know about the new palette being edited
	UVoxelPalette* Palette = GetPaletteBeingEdited();

	if (Palette != nullptr)
	{
		// Create and display a notification about the new palette being edited
		const FText NotificationErrorText = FText::Format(LOCTEXT("SwitchingToPalette", "Editing {0}"), FText::AsCultureInvariant(Palette->GetName()));
		FNotificationInfo Info(NotificationErrorText);
		Info.ExpireDuration = 2.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

#undef LOCTEXT_NAMESPACE
