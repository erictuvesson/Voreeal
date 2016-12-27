#include "VoreealVolumeImporterPrivatePCH.h"
#include "VoreealImporterModule.h"

#include "Formats/MagicaVoxelImporter.h"
#include "Formats/QubicleBinaryImporter.h"


void FVoreealImporterModule::StartupModule()
{
	// TODO: For some reason it seems I cant store values in the module.
	//RegisterImporter(new FMagicaVoxelImporter());
	//RegisterImporter(new FQubicleBinaryImporter());
}

void FVoreealImporterModule::ShutdownModule()
{
	for (int32 i = 0; i < Importers.Num(); i++)
		delete Importers[i];
	Importers.Empty();
}

void FVoreealImporterModule::RegisterImporter(FVoreealImporter* Importer)
{
	Importers.Add(Importer);
}

IMPLEMENT_MODULE(FVoreealImporterModule, VoreealImporter);

DEFINE_LOG_CATEGORY(LogVoreealImporter);
