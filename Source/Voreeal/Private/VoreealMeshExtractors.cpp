#include "VoreealPrivatePCH.h"
#include "VoreealMeshExtractors.h"

void UVoreealCubicSurfaceExtractor::ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options, FProcMeshSection& OutSection)
{
	/*VolumeType* processVolume = Volume;

	auto mesh = extractCubicMesh(processVolume, region);
	ConvertMesh(mesh, Mesh);*/

}

void UVoreealMarchingCubesSurfaceExtractor::ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options, FProcMeshSection& OutSection)
{
	/*VolumeType* processVolume = Volume;

	auto mesh = extractMarchingCubesMesh(processVolume, region);
	ConvertMesh(mesh, Mesh);*/
}
