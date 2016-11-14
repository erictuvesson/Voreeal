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

	//if (bShowSockets && !IsInSourceRegionEditMode())
	//{
	//	FSpriteGeometryEditMode* GeometryEditMode = ModeTools->GetActiveModeTyped<FSpriteGeometryEditMode>(FSpriteGeometryEditMode::EM_SpriteGeometry);
	//	FSocketEditingHelper::DrawSocketNames(GeometryEditMode, RenderVolumeComponent, Viewport, View, Canvas);
	//}

	FEditorViewportClient::DrawCanvas(pViewport, View, Canvas);
}

void FBasicVolumeEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);

	//// We don't draw the pivot when showing the source region
	//// The pivot may be outside the actual texture bounds there
	//if (bShowPivot && !bShowSourceTexture && !IsInSourceRegionEditMode())
	//{
	//	const bool bCanSelectPivot = false;
	//	const bool bHitTestingForPivot = PDI->IsHitTesting() && bCanSelectPivot;
	//	FUnrealEdUtils::DrawWidget(View, PDI, RenderSpriteComponent->ComponentToWorld.ToMatrixWithScale(), 0, 0, EAxisList::XZ, EWidgetMovementMode::WMM_Translate, bHitTestingForPivot);
	//}

	//if (bShowSockets && !IsInSourceRegionEditMode())
	//{
	//	FSpriteGeometryEditMode* GeometryEditMode = ModeTools->GetActiveModeTyped<FSpriteGeometryEditMode>(FSpriteGeometryEditMode::EM_SpriteGeometry);
	//	FSocketEditingHelper::DrawSockets(GeometryEditMode, RenderSpriteComponent, View, PDI);
	//}
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
		//@TODO: Doesn't need to happen every frame, only when properties are updated

	}

	FVoreealEditorViewportClient::Tick(DeltaSeconds);

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

	//HSpriteSelectableObjectHitProxy* SelectedItemProxy = HitProxyCast<HSpriteSelectableObjectHitProxy>(HitProxy);
	//
	//if (IsInSourceRegionEditMode())
	//{
	//	if ((Event == EInputEvent::IE_DoubleClick) && (Key == EKeys::LeftMouseButton))
	//	{
	//		FVector4 WorldPoint = View.PixelToWorld(HitX, HitY, 0);
	//		UPaperSprite* Sprite = GetSpriteBeingEdited();
	//		FVector2D TexturePoint = SourceTextureViewComponent->GetSprite()->ConvertWorldSpaceToTextureSpace(WorldPoint);
	//		if (bIsCtrlKeyDown)
	//		{
	//			const FVector2D StartingUV = Sprite->GetSourceUV();
	//			const FVector2D StartingSize = Sprite->GetSourceSize();
	//
	//			if (UPaperSprite* NewSprite = CreateNewSprite(FIntPoint((int32)StartingUV.X, (int32)StartingUV.Y), FIntPoint((int32)StartingSize.X, (int32)StartingSize.Y)))
	//			{
	//				NewSprite->ExtractSourceRegionFromTexturePoint(TexturePoint);
	//				bHandled = true;
	//			}
	//		}
	//		else
	//		{
	//			Sprite->ExtractSourceRegionFromTexturePoint(TexturePoint);
	//			bHandled = true;
	//		}
	//	}
	//	else if ((Event == EInputEvent::IE_Released) && (Key == EKeys::LeftMouseButton))
	//	{
	//		FVector4 WorldPoint = View.PixelToWorld(HitX, HitY, 0);
	//		FVector2D TexturePoint = SourceTextureViewComponent->GetSprite()->ConvertWorldSpaceToTextureSpace(WorldPoint);
	//		for (int32 RelatedSpriteIndex = 0; RelatedSpriteIndex < RelatedSprites.Num(); ++RelatedSpriteIndex)
	//		{
	//			FRelatedSprite& RelatedSprite = RelatedSprites[RelatedSpriteIndex];
	//			if ((TexturePoint.X >= RelatedSprite.SourceUV.X) && (TexturePoint.Y >= RelatedSprite.SourceUV.Y) &&
	//				(TexturePoint.X < (RelatedSprite.SourceUV.X + RelatedSprite.SourceDimension.X)) &&
	//				(TexturePoint.Y < (RelatedSprite.SourceUV.Y + RelatedSprite.SourceDimension.Y)))
	//			{
	//				bHandled = true;
	//
	//				// Select this sprite
	//				if (UPaperSprite* LoadedSprite = Cast<UPaperSprite>(RelatedSprite.AssetData.GetAsset()))
	//				{
	//					if (SpriteEditorPtr.IsValid())
	//					{
	//						SpriteEditorPtr.Pin()->SetSpriteBeingEdited(LoadedSprite);
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

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
	//@TODO: Should push this into FEditorViewportClient
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
	return FLinearColor();
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
