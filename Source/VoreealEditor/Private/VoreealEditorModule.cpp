#include "VoreealEditorPrivatePCH.h"
#include "VoreealEditorModule.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
// TODO: Commands etc

#include "AssetEditorToolkit.h"
#include "ModuleManager.h"
//#include "ContentBrowserExtensions/ContentBrowserExtensions.h"
// TODO: Settings

// Voreeal Support 
#include "VBasicVolume.h"

// Intro tutorials
#include "Editor/IntroTutorials/Public/IIntroTutorials.h"

class FVoreealEditor : public IVoreealEditorModule
{
public:
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_MODULE(FVoreealEditor, VoreealEditor);
