#include "VoreealEditorPrivatePCH.h"
#include "VImporterSettings.h"

UVImporterSettings::UVImporterSettings()
{
	if (!IsRunningCommandlet())
	{
		//BaseMaterialName = FStringAssetReference("/Voreeal/VertexColorMaterial");
	}
}
