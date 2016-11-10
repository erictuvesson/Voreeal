#include "VoreealEditorPrivatePCH.h"
#include "VoreealImporterSettings.h"

UVoreealImporterSettings::UVoreealImporterSettings()
{
	if (!IsRunningCommandlet())
	{
		//BaseMaterialName = FStringAssetReference("/Voreeal/VertexColorMaterial");
	}
}
