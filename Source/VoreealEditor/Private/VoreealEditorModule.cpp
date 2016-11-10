#include "VoreealEditorPrivatePCH.h"
#include "VoreealEditorModule.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"

#include "AssetEditorToolkit.h"
#include "ModuleManager.h"

// Voreeal Support 
#include "VoreealBasicVolume.h"

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
