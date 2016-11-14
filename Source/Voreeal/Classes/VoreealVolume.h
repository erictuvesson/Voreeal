#pragma once

#include <ProceduralMeshComponent.h>

#include "VoreealRegion.h"
#include "VoreealVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVolumeChanged, FVoreealRegion, Region);

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
	// Used to identify for example which node it was in a octree.
	int32 Identifier;

	// The region to extract.
	FVoreealRegion Region;

	// The level of detail we are extracting.
	int32 LOD;

	FVoreealExtractorOptions(int32 Identifier, FVoreealRegion Region, int32 LOD)
		: Identifier(Identifier)
		, Region(Region)
		, LOD(LOD)
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

/** Base Voxel Volume, used as a data format. */
UCLASS(Abstract)
class VOREEAL_API UVoreealVolume : public UObject
{
	GENERATED_BODY()

public:
	// Occures when the volume has changed.
	UPROPERTY(BlueprintAssignable, Category = "Voreeal")
	FVolumeChanged OnChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Voreeal")
	EVolumeExtractor ExtractorType;

public:
	// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void SetVoxelXYZ(const int32& X, const int32& Y, const int32& Z, const FColor& Color);

	// Set Voxel at Location.
	void SetVoxel(const FIntVector& Location, const FColor& Color);

	// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void SetVoxel(const FVector& Location, const FColor& Color);
	
	// Get Voxel at Location.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	void GetVoxel(const FVector& Location, FColor& Color);

	// Checks if the volume is valid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	virtual bool IsValid() const;

	// Extract part of the volume.
	virtual FVoreealMesh ExtractMesh(const FVoreealExtractorOptions& Options);

public:
	void SerializeVolume(FArchive& Ar, FVoreealRegion& Region);
	void DeserializeVolume(FArchive& Ar);

protected:
	virtual bool Internal_SetVoxel(const FVector& Location, const uint32& Data);
	virtual void Internal_GetVoxel(const FVector& Location, uint32& Data);
	virtual void Internal_SetSize(const FVoreealRegion& Region, bool New);
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

	// TODO: LOD

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
			Result.Normals.Add(VertexNormal);
			
			FColor Color;
			Color.R = (vertex.data & 0x000000FF);
			Color.G = (vertex.data & 0x0000FF00) >> 8;
			Color.B = (vertex.data & 0x00FF0000) >> 16;
			Color.A = (vertex.data & 0xFF000000) >> 24;

			Result.VertexColors.Add(Color);
		}

		for (int32 i = 0; i < (int32)mesh.getNoOfIndices(); i += 3)
		{
			auto Index1 = mesh.getIndex(i);
			auto Index2 = mesh.getIndex(i + 1);
			auto Index3 = mesh.getIndex(i + 2);

			Result.Triangles.Add((int32)Index1);
			Result.Triangles.Add((int32)Index2);
			Result.Triangles.Add((int32)Index3);
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
