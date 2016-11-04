#pragma once

#include "FileFormat.h"

///  MagicaVoxel importer
class FMagicaVoxelFileFormat : public FVFileFormat
{
public:
	// Begin FVFileFormat
	virtual FString GetExtension() const override { return "vox"; }
	virtual FString GetFileDescription() const override { return "MagicaVoxel"; }
	virtual bool IsFile(FArchive& Ar, FString& ErrorMessage) override;
	virtual bool ReadFile(FArchive& Ar, UBasicVolume* Volume, FString& ErrorMessage) override;
	// End FVFileFormat
};
