#pragma once

#include "VoreealRegion.h"
#include "VoreealMeshExtractors.generated.h"

class UVoreealVolume;

struct FVoreealMesh
{

};

/// Extractor Options
USTRUCT()
struct FVoreealExtractorOptions
{
	GENERATED_BODY()

	FVoreealRegion Region;
	int32 LOD;
};

/// Base Class for mesh extractors.
/// We could change this to a interface instead, just for now there is no reason to.
UCLASS(Abstract)
class UVoreealMeshExtractor : public UObject
{
	GENERATED_BODY()

public:
	/// Extract the mesh.
	virtual void ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options, FProcMeshSection& OutSection) { }
};

/// Cubic Surface Extractor, extracts the voxels as cubes like Minecraft.
UCLASS()
class UVoreealCubicSurfaceExtractor : public UVoreealMeshExtractor
{
	GENERATED_BODY()

protected:
	virtual void ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options, FProcMeshSection& OutSection)  override;
};

/// Marching Cubes Surface Extractor, extracts the voxels as smooth terrain.
UCLASS()
class UVoreealMarchingCubesSurfaceExtractor : public UVoreealMeshExtractor
{
	GENERATED_BODY()

protected:
	virtual void ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options, FProcMeshSection& OutSection) override;
};
