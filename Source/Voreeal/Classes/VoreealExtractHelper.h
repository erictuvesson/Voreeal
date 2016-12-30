#pragma once

#include <PolyVox/RawVolume.h>
#include <PolyVox/VolumeResampler.h>

#include "VoreealRegion.h"
#include "VoreealOctree.h"
#include "VoreealExtractHelper.generated.h"

UENUM(BlueprintType)
enum class EVolumeExtractor : uint8
{
	// Cubic Surface Extractor, extracts the voxels as cubes like Minecraft.
	CubicSurface,
	// Marching Cubes Surface Extractor, extracts the voxels as smooth terrain.
	MarchingCubesSurface
};

// Extractor Options
struct VOREEAL_API FVoreealExtractorOptions
{
	// A pointer to the owning octree.
	TWeakPtr<FSparseOctree> Octree;

	// identify for example which node it was in a octree.
	int32 Identifier;

	// identifier for paged volume.
	FIntVector PagedIdentifier;

	// The region to extract.
	FVoreealRegion Region;

	// The level of detail we are extracting.
	int32 LOD;

	// Override the extractor type.
	uint32 bOverrideExtractor : 1;

	// Extractor type to override.
	EVolumeExtractor ExtractorType;

	FVoreealExtractorOptions(TWeakPtr<FSparseOctree> Octree, int32 Identifier, FVoreealRegion Region, int32 LOD)
		: Octree(Octree)
		, Identifier(Identifier)
		, Region(Region)
		, LOD(LOD)
		, bOverrideExtractor(false)
		, ExtractorType(EVolumeExtractor::CubicSurface)
	{

	}
};

// Extracted Mesh
class VOREEAL_API FVoreealMesh
{
public:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

public:
	FVoreealMesh(FVoreealExtractorOptions Options)
		: Options(Options)
	{

	}

	// Add a vertex.
	void AddVertex(FVector Position, FVector Normal, FVector2D UV, FColor Color, FProcMeshTangent Tangent);

	// Set the section.
	void CreateSection(class UProceduralMeshComponent* ProceduralMeshComponent, bool bCreateCollision);

	// Gets the options used for this extraction.
	FVoreealExtractorOptions GetOptions() const;

private:
	FVoreealExtractorOptions Options;
};

// Helper class for extracting meshes
// I just dont want to write the same code twice
template <typename TVolume, typename TVoxelType>
class VoreealExtractHelper
{
public:
	typedef TVoxelType	VoxelType;
	typedef TVolume		VolumeType;

	FVoreealMesh ExtractMeshHelper(TVolume* Volume, EVolumeExtractor ExtractorType, const FVoreealExtractorOptions& Options);
};

template <typename TVolume, typename TVoxelType>
inline FVoreealMesh VoreealExtractHelper<TVolume, TVoxelType>::ExtractMeshHelper(TVolume* Volume, EVolumeExtractor ExtractorType, const FVoreealExtractorOptions& Options)
{
	PolyVox::Region PRegion = (PolyVox::Region)Options.Region;
	FVector RegionLower = Options.Region.GetLower().GetAbs();

	FVoreealMesh Result = FVoreealMesh(Options);

	// Process LOD
	//PolyVox::RawVolume<TVoxelType> VolumeLOD(PRegion);
	//
	//PolyVox::VolumeResampler<PolyVox::RawVolume<TVoxelType>, TVolume> VolumeResampler(&Volume, /* Scale */, &VolumeLOD, VolumeLOD.getEnclosingRegion());
	//VolumeResampler.execute();


	// TODO: handle voxel data
	switch (ExtractorType)
	{
	case EVolumeExtractor::CubicSurface:
	{
		auto mesh = extractCubicMesh(Volume, PRegion);

		for (int32 i = 0; i < (int32)mesh.getNoOfVertices(); i++)
		{
			auto vertex = decodeVertex(mesh.getVertex(i));

			FVector VertexPosition = FVector(vertex.position.getX(), vertex.position.getY(), vertex.position.getZ());
			FVector VertexNormal = FVector(vertex.normal.getX(), vertex.normal.getY(), vertex.normal.getZ());
			// Normals are not supported by CubicSurface Extractor, so this should probably be removed.

			Result.Vertices.Add(RegionLower + VertexPosition);
			//Result.Normals.Add(VertexNormal);

			FColor Color;
			Color.R = (vertex.data.getMaterial() & 0x000000FF);
			Color.G = (vertex.data.getMaterial() & 0x0000FF00) >> 8;
			Color.B = (vertex.data.getMaterial() & 0x00FF0000) >> 16;
			Color.A = 255; // (vertex.data & 0xFF000000) >> 24;
			
			Result.VertexColors.Add(Color);
		}

		for (int32 i = 0; i < (int32)mesh.getNoOfIndices(); i += 3)
		{
			auto Index1 = mesh.getIndex(i);
			auto Index2 = mesh.getIndex(i + 1);
			auto Index3 = mesh.getIndex(i + 2);

			Result.Triangles.Add((int32)Index3);
			Result.Triangles.Add((int32)Index2);
			Result.Triangles.Add((int32)Index1);

			// Make Normal
			auto Vertex1 = Result.Vertices[Index3];
			auto Vertex2 = Result.Vertices[Index2];
			auto Vertex3 = Result.Vertices[Index1];

			FVector Direction = FVector::CrossProduct(Vertex2 - Vertex1, Vertex3 - Vertex1);
			Direction.Normalize();

			Result.Normals.Add(Direction);
		}

		break;
	}
	case EVolumeExtractor::MarchingCubesSurface:
	{
		auto mesh = extractMarchingCubesMesh(Volume, PRegion);

		for (int32 i = 0; i < (int32)mesh.getNoOfVertices(); i++)
		{
			auto vertex = decodeVertex(mesh.getVertex(i));

			FVector VertexPosition = FVector(vertex.position.getX(), vertex.position.getY(), vertex.position.getZ());
			FVector VertexNormal = FVector(vertex.normal.getX(), vertex.normal.getY(), vertex.normal.getZ());

			Result.Vertices.Add(RegionLower + VertexPosition);
			Result.Normals.Add(VertexNormal);
			Result.VertexColors.Add(FColor(vertex.data.getMaterial()));
		}

		for (int32 i = 0; i < (int32)mesh.getNoOfIndices(); i += 3)
		{
			auto Index1 = mesh.getIndex(i);
			auto Index2 = mesh.getIndex(i + 1);
			auto Index3 = mesh.getIndex(i + 2);

			Result.Triangles.Add((int32)Index3);
			Result.Triangles.Add((int32)Index2);
			Result.Triangles.Add((int32)Index1);
		}

		break;
	}
	}

	return Result;
}
