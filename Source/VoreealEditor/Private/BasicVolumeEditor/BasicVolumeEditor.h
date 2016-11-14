#pragma once

#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/AssetEditorManager.h"

class SBasicVolumeEditorViewport;

enum class EBasicVolumeEditorMode
{
	ViewMode,
	EditMode
};

class FBasicVolumeEditor : public FAssetEditorToolkit, public FGCObject
{
public:
	// Begin IToolkit Interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	// End IToolkit Interface

	// Begin FAssetEditorToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override;
	// End FAssetEditorToolkit Interface

	// Begin FSerializableObject Interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End FSerializableObject Interface

public:
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UBasicVolume* InitVolume);

	EBasicVolumeEditorMode GetCurrentMode() const;

	UBasicVolume* GetVolumeBeingEdited() const;
	void SetVolumeBeingEdited(UBasicVolume* NewVolume);

protected:
	UBasicVolume* VolumeBeingEdited;
	TSharedPtr<SBasicVolumeEditorViewport> ViewportPtr;

protected:
	void BindCommands();
	void ExtendMenu();
	void ExtendToolbar();

	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

	void CreateModeToolbarWidgets(FToolBarBuilder& ToolbarBuilder);

	FText GetCurrentModeCornerText() const;
};
