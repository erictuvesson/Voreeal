#include "VoreealPrivatePCH.h"
#include "VoreealMeshExtractors.h"

FVoreealMesh UVoreealCubicSurfaceExtractor::ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options)
{
	/*VolumeType* processVolume = Volume;

	auto mesh = extractCubicMesh(processVolume, region);
	ConvertMesh(mesh, Mesh);*/

	return FVoreealMesh(Options);
}

FVoreealMesh UVoreealMarchingCubesSurfaceExtractor::ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options)
{
	/*VolumeType* processVolume = Volume;

	auto mesh = extractMarchingCubesMesh(processVolume, region);
	ConvertMesh(mesh, Mesh);*/

	return FVoreealMesh(Options);
}
