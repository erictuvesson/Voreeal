#pragma once

#include "ModuleManager.h"

class IVoreealModule : public IModuleInterface
{
public:
	inline static IVoreealModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IVoreealModule>("Voreeal");
	}

	inline static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("Voreeal");
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoreeal, Log, All);
