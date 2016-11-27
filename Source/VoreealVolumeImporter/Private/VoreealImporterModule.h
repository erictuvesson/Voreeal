#pragma once

#include "VoreealImporter.h"

class FVoreealImporterModule : public IVoreealImporterModule
{
public:
	// Begin IModuleInterface Interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface Interface

	// Begin IVoreealImporterModule Interface
	virtual void RegisterImporter(FVoreealImporter* Importer);
	virtual const TArray<FVoreealImporter*> GetImporters() const { return Importers; }
	// End IVoreealImporterModule Interface

private:
	TArray<FVoreealImporter*> Importers;
};
