#include "VoreealPrivatePCH.h"
#include "VoreealVolume.h"

void FVoreealMesh::AddVertex(FVector Position, FVector Normal, FVector2D UV, FColor Color, FProcMeshTangent Tangent)
{
	Vertices.Add(Position);
	Normals.Add(Normal);
	UV0.Add(UV);
	VertexColors.Add(Color);
	Tangents.Add(Tangent);
}

void FVoreealMesh::CreateSection(UProceduralMeshComponent* ProceduralMeshComponent, bool bCreateCollision)
{
	ProceduralMeshComponent->CreateMeshSection(
		Options.Identifier, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, bCreateCollision);
}

FVoreealExtractorOptions FVoreealMesh::GetOptions() const
{
	return Options;
}

void UVoreealVolume::SetVoxelXYZ(const int32& X, const int32& Y, const int32& Z, const uint8& Material, const uint8& Density)
{
	SetVoxel(FVector(X, Y, Z), Material, Density);
}

void UVoreealVolume::SetVoxel(const FIntVector& Location, const uint8& Material, const uint8& Density)
{
	SetVoxel(FVector(Location.X, Location.Y, Location.Z), Material, Density);
}

void UVoreealVolume::SetVoxel(const FVector& Location, const uint8& Material, const uint8& Density)
{
	Internal_SetVoxel(Location, Material, Density);
}

void UVoreealVolume::GetVoxel(const FVector& Location, uint8& Material, uint8& Density)
{
	Internal_GetVoxel(Location, Material, Density);
}

bool UVoreealVolume::IsValid() const
{
	check(0 && "abstract");
	return false;
}

FVoreealMesh UVoreealVolume::ExtractMesh(const FVoreealExtractorOptions& Options)
{
	check(0 && "abstract");
	return FVoreealMesh(Options);
}

bool UVoreealVolume::Internal_SetVoxel(const FVector& Location, const uint8& Material, const uint8& Density)
{
	check(0 && "abstract");
	return false;
}

void UVoreealVolume::Internal_GetVoxel(const FVector& Location, uint8& Material, uint8& Density)
{
	check(0 && "abstract");
}

void UVoreealVolume::Internal_SetSize(const FVoreealRegion& Region, bool New)
{
	check(0 && "abstract");
}

void UVoreealVolume::SerializeVolume(FArchive& Ar, FVoreealRegion& Region)
{
	// TODO: Write Version
	Ar << Region;

	uint8 Material;
	uint8 Density;
	for (int32 x = Region.X; x < (Region.X + Region.Width); x++)
	{
		for (int32 y = Region.Y; y < (Region.Y + Region.Height); y++)
		{
			for (int32 z = Region.Z; z < (Region.Z + Region.Depth); z++)
			{
				Internal_GetVoxel(FVector(x, y, z), Material, Density);
				Ar << Material << Density;
			}
		}
	}
}

void UVoreealVolume::DeserializeVolume(FArchive& Ar)
{
	// TODO: Write Version
	FVoreealRegion region;
	Ar << region;

	Internal_SetSize(region, true);

	uint8 Material;
	uint8 Density;
	for (int32 x = region.X; x < (region.X + region.Width); x++)
	{
		for (int32 y = region.Y; y < (region.Y + region.Height); y++)
		{
			for (int32 z = region.Z; z < (region.Z + region.Depth); z++)
			{
				Ar << Material << Density;
				Internal_SetVoxel(FVector(x, y, z), Material, Density);
			}
		}
	}

	OnChanged.Broadcast(region);
}
