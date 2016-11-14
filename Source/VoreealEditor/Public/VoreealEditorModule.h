#pragma once

#include "ModuleManager.h"

class IVoreealEditorModule : public IModuleInterface
{
public:
	virtual TSharedPtr<class FExtensibilityManager> GetBasicVolumeEditorMenuExtensibilityManager() { return nullptr; }
	virtual TSharedPtr<class FExtensibilityManager> GetBasicVolumeEditorToolBarExtensibilityManager() { return nullptr; }
};
