#pragma once

#include "VBasicVolume.h"

namespace Voreeal
{
	/**
	 * Import Qubicle Binary file. 
	 * Extension: *.qb
	 */
	UBasicVolume::VolumeType* ImportQubicleBinary(FArchive& Ar);

	/**
	 * Import Magica Voxel file.
	 * Extension: *.vox
	 */
	UBasicVolume::VolumeType* ImportMagicaVoxel(FArchive& Ar);

	/**
	 * Import Voxel Volume.
	 *
	 * @return: nullptr if unsuccessful.
	 */
	UBasicVolume::VolumeType* Import(const FString& CurrentFilename);
}
