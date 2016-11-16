#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeEditor.h"

#include "BasicVolumeEditorViewportClient.h"
#include "BasicVolumeEditorCommands.h"
#include "BasicVolumeComponentDetailsCustomization.h"

#include "SBasicVolumeEditorViewportToolbar.h"
#include "VoreealEditorModule.h"

#include "SSingleObjectDetailsPanel.h"
#include "SceneViewport.h"

#include "GraphEditor.h"

#include "SEditorViewport.h"
#include "WorkspaceMenuStructureModule.h"

#include "SDockTab.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

const FName BasicVolumeEditorAppName = FName(TEXT("BasicVolumeEditorApp"));

struct FBasicVolumeEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName ViewportID;
};

const FName FBasicVolumeEditorTabs::DetailsID(TEXT("Details"));
const FName FBasicVolumeEditorTabs::ViewportID(TEXT("Viewport"));

class SBasicVolumeEditorViewport : public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SBasicVolumeEditorViewport) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FBasicVolumeEditor> InEditor);

	// Begin SEditorViewport Interface
	virtual void BindCommands() override;
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;
	virtual EVisibility GetTransformToolbarVisibility() const override;
	virtual void OnFocusViewportToSelection() override;
	// End SEditorViewport Interface

	// Begin ICommonEditorViewportToolbarInfoProvider Interface
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;
	// End ICommonEditorViewportToolbarInfoProvider Interface

	// Invalidate any references to the volume being edited; it has changed
	void NotifyVolumeBeingEditedHasChanged()
	{
		EditorViewportClient->NotifyVolumeBeingEditedHasChanged();
	}

	EBasicVolumeEditorMode GetCurrentMode() const
	{
		return EditorViewportClient->GetCurrentMode();
	}

	void ActivateEditMode()
	{
		EditorViewportClient->ActivateEditMode();
	}

private:
	// Pointer back to owning editor instance (the keeper of state)
	TWeakPtr<class FBasicVolumeEditor> EditorPtr;

	// Viewport client
	TSharedPtr<FBasicVolumeEditorViewportClient> EditorViewportClient;
};

void SBasicVolumeEditorViewport::Construct(const FArguments& InArgs, TSharedPtr<FBasicVolumeEditor> InEditor)
{
	EditorPtr = InEditor;

	SEditorViewport::Construct(SEditorViewport::FArguments());
}

void SBasicVolumeEditorViewport::BindCommands()
{
	SEditorViewport::BindCommands();

	const FBasicVolumeEditorCommands& Commands = FBasicVolumeEditorCommands::Get();

	TSharedRef<FBasicVolumeEditorViewportClient> EditorViewportClientRef = EditorViewportClient.ToSharedRef();

	// Show toggles
	CommandList->MapAction(
		Commands.SetShowGrid,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FEditorViewportClient::SetShowGrid),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FEditorViewportClient::IsSetShowGridChecked));

	CommandList->MapAction(
		Commands.SetShowBounds,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FEditorViewportClient::ToggleShowBounds),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FEditorViewportClient::IsSetShowBoundsChecked));

	CommandList->MapAction(
		Commands.SetShowOctree,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::ToggleShowOctree),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::IsSetShowOctreeChecked));

	CommandList->MapAction(
		Commands.SetShowSockets,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::ToggleShowSockets),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::IsShowSocketsChecked));

	//CommandList->MapAction(
	//	Commands.SetShowPivot,
	//	FExecuteAction::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::ToggleShowPivot),
	//	FCanExecuteAction(),
	//	FIsActionChecked::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::IsShowPivotChecked));

	// Editing modes
	CommandList->MapAction(
		Commands.EnterViewMode,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::EnterViewMode),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::IsInViewMode));

	CommandList->MapAction(
		Commands.EnterEditMode,
		FExecuteAction::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::EnterEditMode),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(EditorViewportClientRef, &FBasicVolumeEditorViewportClient::IsInEditMode));
}

TSharedRef<FEditorViewportClient> SBasicVolumeEditorViewport::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FBasicVolumeEditorViewportClient(EditorPtr, SharedThis(this)));

	return EditorViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SBasicVolumeEditorViewport::MakeViewportToolbar()
{
	return SNew(SBasicVolumeEditorViewportToolbar, SharedThis(this));
}

EVisibility SBasicVolumeEditorViewport::GetTransformToolbarVisibility() const
{
	return EVisibility::Visible;
}

void SBasicVolumeEditorViewport::OnFocusViewportToSelection()
{
	//EditorViewportClient->RequestFocusOnSelection(/*bInstant=*/ false);
}

TSharedRef<class SEditorViewport> SBasicVolumeEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SBasicVolumeEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SBasicVolumeEditorViewport::OnFloatingButtonClicked()
{

}

class SBasicVolumePropertiesTabBody : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SBasicVolumePropertiesTabBody) {}
	SLATE_END_ARGS()

private:
	// Pointer back to owning editor instance (the keeper of state)
	TWeakPtr<class FBasicVolumeEditor> EditorPtr;

public:
	void Construct(const FArguments& InArgs, TSharedPtr<FBasicVolumeEditor> InEditor)
	{
		EditorPtr = InEditor;

		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InEditor->GetToolkitCommands()), 
			/*bAutomaticallyObserveViaGetObjectToObserve=*/ true, /*bAllowSearch=*/ true);

		TAttribute<EBasicVolumeEditorMode> EditorMode = TAttribute<EBasicVolumeEditorMode>::Create(
			TAttribute<EBasicVolumeEditorMode>::FGetter::CreateSP(InEditor.ToSharedRef(), &FBasicVolumeEditor::GetCurrentMode));

		FOnGetDetailCustomizationInstance CustomizeForEditor = FOnGetDetailCustomizationInstance::CreateStatic(
			&FBasicVolumeDetailsCustomization::MakeInstanceForEditor, EditorMode);
		PropertyView->RegisterInstancedCustomPropertyLayout(UBasicVolume::StaticClass(), CustomizeForEditor);
	}

	// Begin SSingleObjectDetailsPanel Interface
	virtual UObject* GetObjectToObserve() const override
	{
		return EditorPtr.Pin()->GetVolumeBeingEdited();
	}

	virtual TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override
	{
		return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(1)
			[
				PropertyEditorWidget
			];
	}
	// End SSingleObjectDetailsPanel Interface
};

TSharedRef<SDockTab> FBasicVolumeEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"))
		[
			SNew(SOverlay)
			+ SOverlay::Slot() //< The editor viewport
			[
				ViewportPtr.ToSharedRef()
			]
			// Bottom-right corner text indicating the preview nature of the sprite editor
			+ SOverlay::Slot()
			.Padding(10)
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			[
				SNew(STextBlock)
				.Visibility(EVisibility::HitTestInvisible)
				.TextStyle(FEditorStyle::Get(), "Graph.CornerText")
				.Text(this, &FBasicVolumeEditor::GetCurrentModeCornerText)
			]
		];
}

TSharedRef<SDockTab> FBasicVolumeEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	TSharedPtr<FBasicVolumeEditor> EditorPtr = SharedThis(this);
	return SNew(SDockTab)
		.Label(LOCTEXT("DetailsTab_Title", "Details"))
		[
			SNew(SBasicVolumePropertiesTabBody, EditorPtr)
		];
}

void FBasicVolumeEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& pTabManager)
{
	WorkspaceMenuCategory = pTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SpriteEditor", "Voxel Volume Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(pTabManager);

	pTabManager->RegisterTabSpawner(FBasicVolumeEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FBasicVolumeEditor::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("ViewportTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	pTabManager->RegisterTabSpawner(FBasicVolumeEditorTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FBasicVolumeEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTabLabel", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FBasicVolumeEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& pTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(pTabManager);

	pTabManager->UnregisterTabSpawner(FBasicVolumeEditorTabs::ViewportID);
	pTabManager->UnregisterTabSpawner(FBasicVolumeEditorTabs::DetailsID);
}

void FBasicVolumeEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UBasicVolume* InitVolume)
{
	VolumeBeingEdited = InitVolume;

	FBasicVolumeEditorCommands::Register();

	BindCommands();

	TSharedPtr<FBasicVolumeEditor> EditorPtr = SharedThis(this);
	ViewportPtr = SNew(SBasicVolumeEditorViewport, EditorPtr);

	// Default layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_BasicVolumeEditor_Layout_v6")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.8f)
					->SetHideTabWell(true)
					->AddTab(FBasicVolumeEditorTabs::ViewportID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.2f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.75f)
						->SetHideTabWell(true)
						->AddTab(FBasicVolumeEditorTabs::DetailsID, ETabState::OpenedTab)
					)
				)
			)
		);

	// Initialize the asset editor
	InitAssetEditor(Mode, InitToolkitHost, BasicVolumeEditorAppName, StandaloneDefaultLayout, 
		/*bCreateDefaultStandaloneMenu=*/ true, /*bCreateDefaultToolbar=*/ true, InitVolume);

	ViewportPtr->ActivateEditMode();

	// Extend things
	ExtendMenu();
	ExtendToolbar();
	RegenerateMenusAndToolbars();
}

void FBasicVolumeEditor::BindCommands()
{

}

FName FBasicVolumeEditor::GetToolkitFName() const
{
	return FName("BasicVolumeEditor");
}

FText FBasicVolumeEditor::GetBaseToolkitName() const
{
	return LOCTEXT("BasicVolumeEditorAppLabel", "Basic Volume Editor");
}

FText FBasicVolumeEditor::GetToolkitName() const
{
	const bool bDirtyState = VolumeBeingEdited->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("BasicVolumeName"), FText::FromString(VolumeBeingEdited->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("BasicVolumeEditorAppLabel", "{BasicVolumeName}{DirtyState}"), Args);
}

FText FBasicVolumeEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(VolumeBeingEdited);
}

FString FBasicVolumeEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("BasicVolumeEditor");
}

FString FBasicVolumeEditor::GetDocumentationLink() const
{
	// TODO: 
	return TEXT("Voreeal/BasicVolumeEditor");
}

void FBasicVolumeEditor::OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit)
{
	//@TODO: MODETOOLS: Need to be able to register the widget in the toolbox panel with ToolkitHost, so it can instance the ed mode widgets into it
	// 	TSharedPtr<SWidget> InlineContent = Toolkit->GetInlineContent();
	// 	if (InlineContent.IsValid())
	// 	{
	// 		ToolboxPtr->SetContent(InlineContent.ToSharedRef());
	// 	}
}

void FBasicVolumeEditor::OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit)
{
	//ToolboxPtr->SetContent(SNullWidget::NullWidget);
	//@TODO: MODETOOLS: How to handle multiple ed modes at once in a standalone asset editor?
}

FLinearColor FBasicVolumeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Black;
}

void FBasicVolumeEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(VolumeBeingEdited);
}

void FBasicVolumeEditor::ExtendMenu()
{

}

void FBasicVolumeEditor::ExtendToolbar()
{
	struct Local
	{
		static void FillToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			const FBasicVolumeEditorCommands& BasicVolumeCommands = FBasicVolumeEditorCommands::Get();

			ToolbarBuilder.BeginSection("Command");
			{
				//ToolbarBuilder.AddToolBarButton(BasicVolumeCommands.SetShowSourceTexture);
				//ToolbarBuilder.AddToolBarButton(BasicVolumeCommands.ToggleShowRelatedSprites);
				//ToolbarBuilder.AddToolBarButton(BasicVolumeCommands.ToggleShowSpriteNames);
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("Tools");
			{
				//ToolbarBuilder.AddToolBarButton(BasicVolumeCommands.ExtractSprites);
			}
			ToolbarBuilder.EndSection();
		}
	};

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		ViewportPtr->GetCommandList(),
		FToolBarExtensionDelegate::CreateStatic(&Local::FillToolbar)
	);

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		ViewportPtr->GetCommandList(),
		FToolBarExtensionDelegate::CreateSP(this, &FBasicVolumeEditor::CreateModeToolbarWidgets));

	AddToolbarExtender(ToolbarExtender);

	//IVoreealEditorModule* EditorModule = &FModuleManager::LoadModuleChecked<IVoreealEditorModule>("VoreealEditor");
	//AddToolbarExtender(EditorModule->GetSpriteEditorToolBarExtensibilityManager()->GetAllExtenders());
}

void FBasicVolumeEditor::SetVolumeBeingEdited(UBasicVolume* NewVolume)
{
	if ((NewVolume != VolumeBeingEdited) && (NewVolume != nullptr))
	{
		UBasicVolume* OldVolume = VolumeBeingEdited;
		VolumeBeingEdited = NewVolume;

		// Let the viewport know that we are editing something different
		ViewportPtr->NotifyVolumeBeingEditedHasChanged();

		// Let the editor know that are editing something different
		RemoveEditingObject(OldVolume);
		AddEditingObject(NewVolume);
	}
}

EBasicVolumeEditorMode FBasicVolumeEditor::GetCurrentMode() const
{
	return ViewportPtr->GetCurrentMode();
}

UBasicVolume* FBasicVolumeEditor::GetVolumeBeingEdited() const
{
	return VolumeBeingEdited;
}

void FBasicVolumeEditor::CreateModeToolbarWidgets(FToolBarBuilder& IgnoredBuilder)
{
	FToolBarBuilder ToolbarBuilder(ViewportPtr->GetCommandList(), FMultiBoxCustomization::None);
	ToolbarBuilder.AddToolBarButton(FBasicVolumeEditorCommands::Get().EnterViewMode);
	ToolbarBuilder.AddToolBarButton(FBasicVolumeEditorCommands::Get().EnterEditMode);
	AddToolbarWidget(ToolbarBuilder.MakeWidget());
}

FText FBasicVolumeEditor::GetCurrentModeCornerText() const
{
	switch (GetCurrentMode())
	{
	case EBasicVolumeEditorMode::EditMode:
		return LOCTEXT("EditMode_CornerText", "Edit Volume");
	default:
		return FText::GetEmpty();
	}
}

#undef LOCTEXT_NAMESPACE
