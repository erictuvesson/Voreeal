#include "VoreealEditorPrivatePCH.h"
#include "PaletteEditor.h"

#include "PaletteEditorViewportClient.h"
#include "PaletteEditorCommands.h"
#include "PaletteComponentDetailsCustomization.h"

#include "SPaletteEditorViewportToolbar.h"
#include "VoreealEditorModule.h"

#include "SSingleObjectDetailsPanel.h"
#include "SceneViewport.h"

#include "GraphEditor.h"

#include "SEditorViewport.h"
#include "WorkspaceMenuStructureModule.h"

#include "SDockTab.h"

#include "VoreealVoxelPalette.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

const FName PaletteEditorAppName = FName(TEXT("PaletteEditorApp"));

struct FPaletteEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName ViewportID;
};

const FName FPaletteEditorTabs::DetailsID(TEXT("Details"));
const FName FPaletteEditorTabs::ViewportID(TEXT("Viewport"));

class SPaletteEditorViewport : public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SPaletteEditorViewport) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FPaletteEditor> InEditor);

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
	void NotifyPaletteBeingEditedHasChanged()
	{
		EditorViewportClient->NotifyPaletteBeingEditedHasChanged();
	}

private:
	// Pointer back to owning editor instance (the keeper of state)
	TWeakPtr<class FPaletteEditor> EditorPtr;

	// Viewport client
	TSharedPtr<FPaletteEditorViewportClient> EditorViewportClient;
};

void SPaletteEditorViewport::Construct(const FArguments& InArgs, TSharedPtr<FPaletteEditor> InEditor)
{
	EditorPtr = InEditor;

	SEditorViewport::Construct(SEditorViewport::FArguments());
}

void SPaletteEditorViewport::BindCommands()
{
	SEditorViewport::BindCommands();

	const FPaletteEditorCommands& Commands = FPaletteEditorCommands::Get();

	TSharedRef<FPaletteEditorViewportClient> EditorViewportClientRef = EditorViewportClient.ToSharedRef();
}

TSharedRef<FEditorViewportClient> SPaletteEditorViewport::MakeEditorViewportClient()
{
	EditorViewportClient = MakeShareable(new FPaletteEditorViewportClient(EditorPtr, SharedThis(this)));

	return EditorViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SPaletteEditorViewport::MakeViewportToolbar()
{
	return SNew(SPaletteEditorViewportToolbar, SharedThis(this));
}

EVisibility SPaletteEditorViewport::GetTransformToolbarVisibility() const
{
	return EVisibility::Visible;
}

void SPaletteEditorViewport::OnFocusViewportToSelection()
{
	//EditorViewportClient->RequestFocusOnSelection(/*bInstant=*/ false);
}

TSharedRef<class SEditorViewport> SPaletteEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SPaletteEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

void SPaletteEditorViewport::OnFloatingButtonClicked()
{

}

class SPalettePropertiesTabBody : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SPalettePropertiesTabBody) {}
	SLATE_END_ARGS()

private:
	// Pointer back to owning editor instance (the keeper of state)
	TWeakPtr<class FPaletteEditor> EditorPtr;

public:
	void Construct(const FArguments& InArgs, TSharedPtr<FPaletteEditor> InEditor)
	{
		EditorPtr = InEditor;

		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InEditor->GetToolkitCommands()), 
			/*bAutomaticallyObserveViaGetObjectToObserve=*/ true, /*bAllowSearch=*/ true);

		FOnGetDetailCustomizationInstance CustomizeForEditor = FOnGetDetailCustomizationInstance::CreateStatic(
			&FPaletteDetailsCustomization::MakeInstanceForEditor);
		PropertyView->RegisterInstancedCustomPropertyLayout(UVoxelPalette::StaticClass(), CustomizeForEditor);
	}

	// Begin SSingleObjectDetailsPanel Interface
	virtual UObject* GetObjectToObserve() const override
	{
		return EditorPtr.Pin()->GetPaletteBeingEdited();
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

void FPaletteEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& pTabManager)
{
	WorkspaceMenuCategory = pTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SpriteEditor", "Voxel Volume Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(pTabManager);

	pTabManager->RegisterTabSpawner(FPaletteEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FPaletteEditor::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("ViewportTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	pTabManager->RegisterTabSpawner(FPaletteEditorTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FPaletteEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTabLabel", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FPaletteEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& pTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(pTabManager);

	pTabManager->UnregisterTabSpawner(FPaletteEditorTabs::ViewportID);
	pTabManager->UnregisterTabSpawner(FPaletteEditorTabs::DetailsID);
}

FName FPaletteEditor::GetToolkitFName() const
{
	return FName("PaletteEditor");
}

FText FPaletteEditor::GetBaseToolkitName() const
{
	return LOCTEXT("PaletteEditorAppLabel", "Basic Volume Editor");
}

FText FPaletteEditor::GetToolkitName() const
{
	const bool bDirtyState = PaletteBeingEdited->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("PaletteName"), FText::FromString(PaletteBeingEdited->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("PaletteEditorAppLabel", "{PaletteName}{DirtyState}"), Args);
}

FText FPaletteEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(PaletteBeingEdited);
}

FString FPaletteEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("PaletteEditor");
}

FString FPaletteEditor::GetDocumentationLink() const
{
	// TODO: 
	return TEXT("Voreeal/PaletteEditor");
}

void FPaletteEditor::OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit)
{
	//@TODO: MODETOOLS: Need to be able to register the widget in the toolbox panel with ToolkitHost, so it can instance the ed mode widgets into it
	// 	TSharedPtr<SWidget> InlineContent = Toolkit->GetInlineContent();
	// 	if (InlineContent.IsValid())
	// 	{
	// 		ToolboxPtr->SetContent(InlineContent.ToSharedRef());
	// 	}
}

void FPaletteEditor::OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit)
{
	//ToolboxPtr->SetContent(SNullWidget::NullWidget);
	//@TODO: MODETOOLS: How to handle multiple ed modes at once in a standalone asset editor?
}

FLinearColor FPaletteEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Black;
}


void FPaletteEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(PaletteBeingEdited);
}

UVoxelPalette* FPaletteEditor::GetPaletteBeingEdited() const
{
	return PaletteBeingEdited;
}

void FPaletteEditor::SetPaletteBeingEdited(UVoxelPalette* NewPalette)
{
	if ((NewPalette != PaletteBeingEdited) && (NewPalette != nullptr))
	{
		UVoxelPalette* OldPalette = PaletteBeingEdited;
		PaletteBeingEdited = NewPalette;

		// Let the viewport know that we are editing something different
		ViewportPtr->NotifyPaletteBeingEditedHasChanged();

		// Let the editor know that are editing something different
		RemoveEditingObject(OldPalette);
		AddEditingObject(NewPalette);
	}
}

void FPaletteEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class UVoxelPalette* InitPalette)
{
	PaletteBeingEdited = InitPalette;

	FPaletteEditorCommands::Register();

	BindCommands();

	TSharedPtr<FPaletteEditor> EditorPtr = SharedThis(this);
	ViewportPtr = SNew(SPaletteEditorViewport, EditorPtr);

	// Default layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_PaletteEditor_Layout_v6")
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
					->AddTab(FPaletteEditorTabs::ViewportID, ETabState::OpenedTab)
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
						->AddTab(FPaletteEditorTabs::DetailsID, ETabState::OpenedTab)
					)
				)
			)
		);

	// Initialize the asset editor
	InitAssetEditor(Mode, InitToolkitHost, PaletteEditorAppName, StandaloneDefaultLayout,
		/*bCreateDefaultStandaloneMenu=*/ true, /*bCreateDefaultToolbar=*/ true, InitPalette);

	// Extend things
	ExtendMenu();
	ExtendToolbar();
	RegenerateMenusAndToolbars();
}


void FPaletteEditor::BindCommands()
{

}

void FPaletteEditor::ExtendMenu()
{

}

void FPaletteEditor::ExtendToolbar()
{
	struct Local
	{
		static void FillToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			const FPaletteEditorCommands& PaletteCommands = FPaletteEditorCommands::Get();

			ToolbarBuilder.BeginSection("Command");
			{
				//ToolbarBuilder.AddToolBarButton(PaletteCommands.SetShowSourceTexture);
				//ToolbarBuilder.AddToolBarButton(PaletteCommands.ToggleShowRelatedSprites);
				//ToolbarBuilder.AddToolBarButton(PaletteCommands.ToggleShowSpriteNames);
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("Tools");
			{
				//ToolbarBuilder.AddToolBarButton(PaletteCommands.ExtractSprites);
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
		FToolBarExtensionDelegate::CreateSP(this, &FPaletteEditor::CreateModeToolbarWidgets));

	AddToolbarExtender(ToolbarExtender);

	//IVoreealEditorModule* EditorModule = &FModuleManager::LoadModuleChecked<IVoreealEditorModule>("VoreealEditor");
	//AddToolbarExtender(EditorModule->GetSpriteEditorToolBarExtensibilityManager()->GetAllExtenders());
}

TSharedRef<SDockTab> FPaletteEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
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
		.Text(this, &FPaletteEditor::GetCurrentModeCornerText)
		]
		];
}

TSharedRef<SDockTab> FPaletteEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	TSharedPtr<FPaletteEditor> EditorPtr = SharedThis(this);
	return SNew(SDockTab)
		.Label(LOCTEXT("DetailsTab_Title", "Details"))
		[
			SNew(SPalettePropertiesTabBody, EditorPtr)
		];
}

void FPaletteEditor::CreateModeToolbarWidgets(FToolBarBuilder& IgnoredBuilder)
{
	FToolBarBuilder ToolbarBuilder(ViewportPtr->GetCommandList(), FMultiBoxCustomization::None);
	AddToolbarWidget(ToolbarBuilder.MakeWidget());
}

FText FPaletteEditor::GetCurrentModeCornerText() const
{
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE
