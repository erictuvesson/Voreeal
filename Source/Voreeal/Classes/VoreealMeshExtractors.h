#pragma once

#include "VoreealRegion.h"
#include "VoreealMeshExtractors.generated.h"

class UVoreealVolume;

/// Extractor Options
struct FVoreealExtractorOptions
{
	FVoreealRegion Region;
	int32 LOD;

	FVoreealExtractorOptions(FVoreealRegion Region)
		: Region(Region)
		, LOD(0)
	{

	}
};

/// Extracted Mesh
class FVoreealMesh
{
public:
	FVoreealExtractorOptions Options;

public:
	FVoreealMesh(FVoreealExtractorOptions Options)
		: Options(Options)
	{

	}
};

/// Base Class for mesh extractors.
/// We could change this to a interface instead, just for now there is no reason to.
UCLASS(Abstract)
class UVoreealMeshExtractor : public UObject
{
	GENERATED_BODY()

public:
	/// Extract the mesh.
	virtual FVoreealMesh ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options) 
	{ 
		return FVoreealMesh(Options);
	}
};

/// Cubic Surface Extractor, extracts the voxels as cubes like Minecraft.
UCLASS()
class UVoreealCubicSurfaceExtractor : public UVoreealMeshExtractor
{
	GENERATED_BODY()

public:
	virtual FVoreealMesh ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options) override;
};

/// Marching Cubes Surface Extractor, extracts the voxels as smooth terrain.
UCLASS()
class UVoreealMarchingCubesSurfaceExtractor : public UVoreealMeshExtractor
{
	GENERATED_BODY()

public:
	virtual FVoreealMesh ExtractMesh(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options) override;
};
