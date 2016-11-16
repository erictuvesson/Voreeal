#include "VoreealEditorPrivatePCH.h"
#include "VoreealEditorModule.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"

#include "AssetEditorToolkit.h"
#include "ModuleManager.h"

// Settings
#include "ISettingsModule.h"

// Intro tutorials
#include "Editor/IntroTutorials/Public/IIntroTutorials.h"

// Voreeal Support 
#include "VoreealBasicVolume.h"

// Voreeal Editor Support
#include "VoreealStyle.h"
#include "BasicVolumeAssetTypeActions.h"
#include "VoreealBasicVolumeThumbnailRenderer.h"
#include "BasicVolumeEditor/BasicVolumeDetailsCustomization.h"
#include "BasicVolumeEditor/BasicVolumeComponentDetailsCustomization.h"

#include "VoreealRuntimeSettings.h"
#include "VoreealEditorSettings.h"

DEFINE_LOG_CATEGORY(LogVoreealEditor);

#define LOCTEXT_NAMESPACE "VoreealEditor"

class FVoreealEditor : public IVoreealEditorModule
{
public:
	FVoreealEditor()
	{

	}

	// Begin IVoreealEditorModule Interface
	virtual TSharedPtr<FExtensibilityManager> GetBasicVolumeEditorMenuExtensibilityManager() override { return BasicVolumeEditor_MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetBasicVolumeEditorToolBarExtensibilityManager() override { return BasicVolumeEditor_ToolBarExtensibilityManager; }
	// End IVoreealEditorModule Interface

private:
	TSharedPtr<FExtensibilityManager> BasicVolumeEditor_MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> BasicVolumeEditor_ToolBarExtensibilityManager;

	/** All created asset type actions.  
	 * Cached here so that we can unregister them during shutdown. */
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;

	EAssetTypeCategories::Type VoreealAssetCategoryBit;

public:
	virtual void StartupModule() override
	{
		BasicVolumeEditor_MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		BasicVolumeEditor_ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

		// Register slate style overrides
		FVoreealStyle::Initialize();

		// Register commands
		//FVoreealEditorCommands::Register();

		// Register asset types
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		VoreealAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Voreeal")), LOCTEXT("VoreealAssetCategory", "Voreeal"));

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FBasicVolumeAssetTypeActions(VoreealAssetCategoryBit)));

		// Register the details customizations
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.RegisterCustomClassLayout(UBasicVolume::StaticClass()->GetFName(), 
				FOnGetDetailCustomizationInstance::CreateStatic(&FBasicVolumeDetailsCustomization::MakeInstance));
			PropertyModule.RegisterCustomClassLayout(UBasicVolumeComponent::StaticClass()->GetFName(), 
				FOnGetDetailCustomizationInstance::CreateStatic(&FBasicVolumeComponentDetailsCustomization::MakeInstance));

			PropertyModule.NotifyCustomizationModuleChanged();
		}

		// Register the thumbnail renderers
		UThumbnailManager::Get().RegisterCustomRenderer(UBasicVolume::StaticClass(), UBasicVolumeThumbnailRenderer::StaticClass());

		RegisterSettings();
	}

	virtual void ShutdownModule() override
	{
		BasicVolumeEditor_MenuExtensibilityManager.Reset();
		BasicVolumeEditor_ToolBarExtensibilityManager.Reset();

		if (UObjectInitialized())
		{
			UnregisterSettings();

			// Unregister the thumbnail renderers
			UThumbnailManager::Get().UnregisterCustomRenderer(UBasicVolume::StaticClass());
		}

		// Unregister all the asset types that we registered
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
			{
				AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
			}
		}
		CreatedAssetTypeActions.Empty();

		//FVoreealEditorCommands::Unregister();
		FVoreealStyle::Shutdown();
	}

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}

	void RegisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->RegisterSettings("Project", "Plugins", "Voreeal",
				LOCTEXT("RuntimeSettingsName", "Voreeal"),
				LOCTEXT("RuntimeSettingsDescription", "Configure the Voreeal plugin"),
				GetMutableDefault<UVoreealRuntimeSettings>());

			SettingsModule->RegisterSettings("Editor", "ContentEditors", "VoreealEditor",
				LOCTEXT("BasicVolumeEditorSettingsName", "Voreeal Editor"),
				LOCTEXT("BasicVolumeEditorSettingsDescription", "Configure the look and feel of Voreeal's Editors."),
				GetMutableDefault<UVoreealEditorSettings>());

			//SettingsModule->RegisterSettings("Project", "Editor", "Paper2DImport",
			//	LOCTEXT("PaperImporterSettingsName", "Paper2D - Import"),
			//	LOCTEXT("PaperImporterSettingsDescription", "Configure how assets get imported or converted to sprites."),
			//	GetMutableDefault<UPaperImporterSettings>());
		}
	}

	void UnregisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Editor", "Voreeal");
			SettingsModule->UnregisterSettings("Editor", "ContentEditors", "VoreealEditor");
		}
	}
};

IMPLEMENT_MODULE(FVoreealEditor, VoreealEditor);
