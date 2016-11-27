#pragma once

#include "VoreealImporter.h"

///  MagicaVoxel importer
class FMagicaVoxelImporter : public FVoreealImporter
{
public:
	// Begin FVoreealImporter Interface
	virtual FString GetExtension() const override { return "vox"; }
	virtual FString GetFileDescription() const override { return "MagicaVoxel/Voxlap"; }
	virtual bool Import(FArchive& Ar, UBasicVolume* Volume, FFeedbackContext* Warn) override;
	// End FVoreealImporter Interface
};
