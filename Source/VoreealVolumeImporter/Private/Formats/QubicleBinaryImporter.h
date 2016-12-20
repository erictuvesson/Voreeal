#pragma once

#include "VoreealImporter.h"

///  QubicleBinary importer
class FQubicleBinaryImporter : public FVoreealImporter
{
public:
	// Begin FVoreealImporter Interface
	virtual FString GetExtension() const override { return "qb"; }
	virtual FString GetFileDescription() const override { return "Qubicle Binary"; }
	virtual bool Import(FArchive& Ar, UBasicVolume* Volume, FFeedbackContext* Warn) override;
	// End FVoreealImporter Interface
};
