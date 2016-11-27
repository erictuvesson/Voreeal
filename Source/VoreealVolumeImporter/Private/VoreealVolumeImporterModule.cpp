#include "VoreealVolumeImporterPrivatePCH.h"
#include "VoreealImporterModule.h"

#include "Formats/MagicaVoxelImporter.h"
//#include "Formats/QubicleBinaryImporter.h"


void FVoreealImporterModule::StartupModule()
{
	RegisterImporter(new FMagicaVoxelImporter());
	//RegisterImporter(new QubicleBinaryImporter);
}

void FVoreealImporterModule::ShutdownModule()
{
	// TODO: Delete registered importers
}

void FVoreealImporterModule::RegisterImporter(FVoreealImporter* Importer)
{
	Importers.Add(Importer);
}

IMPLEMENT_MODULE(FVoreealImporterModule, VoreealImporter);

DEFINE_LOG_CATEGORY(LogVoreealImporter);
