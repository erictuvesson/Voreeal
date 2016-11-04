#pragma once

#include "VBasicVolume.h"

/// Abstract voxel file format
class FVFileFormat
{
public:
	/// Gets the file extension. example 'vox'.
	virtual FString GetExtension() const = 0;

	/// Gets the file description.
	virtual FString GetFileDescription() const = 0;

	/// Gets if the archive stream match format.
	/// \return if successful
	virtual bool IsFile(FArchive& Ar, FString& ErrorMessage) = 0;

	/// Reads the file from the stream.
	/// \return if successful
	virtual bool ReadFile(FArchive& Ar, UBasicVolume* Volume, FString& ErrorMessage) = 0;
};
