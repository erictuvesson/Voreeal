#pragma once

#include "ModuleManager.h"

//#import "VoreealImporter.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImportersChanged, bool, bNew)

class UBasicVolume;

// Voreeal importer base class
class FVoreealImporter
{
public:
	// Gets the file extension. example 'vox'.
	virtual FString GetExtension() const = 0;

	// Gets the file description.
	virtual FString GetFileDescription() const = 0;

	// Gets the file format text "{ext};{desc}"
	FString GetFileText() const {
		return FString::Printf(TEXT("%s;%s"), *GetExtension(), *GetFileDescription());
	}

	// Import a volume from stream.
	virtual bool Import(FArchive& Ar, UBasicVolume* Volume, FFeedbackContext* Warn) = 0;
};

/**
 * IVoreealImporterModule& VoreealImporter = IVoreealImporterModule::Get();
 * VoreealImporter.RegisterImporter(new MyImporter);
 */
class IVoreealImporterModule : public IModuleInterface
{
public:
	// Occurs when the registered importers changed.
	//FOnImportersChanged OnImportersChanged;

public:
	// Gets the voreeal importer module.
	inline static IVoreealImporterModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IVoreealImporterModule>("Voreeal");
	}

	// Gets if the voreeal importer module is loaded.
	inline static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("Voreeal");
	}

	// Register a new importer format.
	virtual void RegisterImporter(FVoreealImporter* Importer) = 0;

	// Gets the registered importers.
	virtual const TArray<FVoreealImporter*> GetImporters() const = 0;
};
