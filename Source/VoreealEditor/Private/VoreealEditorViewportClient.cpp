#include "VoreealEditorPrivatePCH.h"
#include "VoreealEditorViewportClient.h"

#include "CanvasTypes.h"
#include "SEditorViewport.h"
#include "PreviewScene.h"
#include "ImageUtils.h"

FVoreealEditorViewportClient::FVoreealEditorViewportClient(const TWeakPtr<SEditorViewport>& InEditorViewportWidget)
	: FEditorViewportClient(new FAssetEditorModeManager(), nullptr, InEditorViewportWidget)
{
	bOwnsModeTools = true;
}

FVoreealEditorViewportClient::~FVoreealEditorViewportClient()
{

}

FLinearColor FVoreealEditorViewportClient::GetBackgroundColor() const
{
	return FLinearColor(0, 128, 0, 0);
}

void FVoreealEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);
}

void FVoreealEditorViewportClient::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEditorViewportClient::AddReferencedObjects(Collector);
}
