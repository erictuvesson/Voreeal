#include "VoreealVolumeImporterPrivatePCH.h"
#include "VImporters.h"

namespace Voreeal
{
	UBasicVolume::VolumeType* Import(const FString& CurrentFilename)
	{
		FString FilenameExtension = FPaths::GetExtension(CurrentFilename);

		TArray<uint8> TheBinaryArray;
		if (!FFileHelper::LoadFileToArray(TheBinaryArray, *CurrentFilename))
		{
			UE_LOG(LogVoreealVolumeImporter, Error, TEXT("Invalid File"));
			return nullptr;
		}

		FMemoryReader Ar = FMemoryReader(TheBinaryArray, true);

		UBasicVolume::VolumeType* Result = nullptr;
		if (FilenameExtension == "qb")
		{
			Result = ImportQubicleBinary(Ar);
		}
		else if (FilenameExtension == "vox")
		{
			Result = ImportMagicaVoxel(Ar);
		}
		else
		{
			UE_LOG(LogVoreealVolumeImporter, Error, TEXT("Invalid File"));
		}

		return Result;
	}
}
