#pragma once

#include "FileFormat.h"

///  QubicleBinary importer
class FQubicleBinaryFileFormat : public FVFileFormat
{
public:
	// Begin FVFileFormat
	virtual FString GetExtension() const override { return "qb"; }
	virtual FString GetFileDescription() const override { return "Qubicle Binary"; }
	virtual bool IsFile(FArchive& Ar, FString& ErrorMessage) override;
	virtual bool ReadFile(FArchive& Ar, UBasicVolume* Volume, FString& ErrorMessage) override;
	// End FVFileFormat
};
