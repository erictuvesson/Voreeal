#include "VoreealPrivatePCH.h"
#include "VMeshExtractors.h"

void UCubicSurfaceExtractor::ExtractMesh(UBaseVolume* Volume, const FRegion& region, FProcMeshSection& OutSection)
{
	/*VolumeType* processVolume = Volume;

	auto mesh = extractCubicMesh(processVolume, region);
	ConvertMesh(mesh, Mesh);*/

}

void UMarchingCubesSurfaceExtractor::ExtractMesh(UBaseVolume* Volume, const FRegion& region, FProcMeshSection& OutSection)
{
	/*VolumeType* processVolume = Volume;

	auto mesh = extractMarchingCubesMesh(processVolume, region);
	ConvertMesh(mesh, Mesh);*/
}
