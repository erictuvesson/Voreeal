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

void UVoreealVolume::SetVoxel(FVector Location, const FColor& Material, const uint8& Density)
{
	// Allow suppress then re-enable
	//OnChanged.Broadcast(FVoreealRegion(Location.X, Location.Y, Location.Z, 1, 1, 1));

	Internal_SetVoxel(Location, Material, Density);
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

bool UVoreealVolume::Internal_SetVoxel(FVector Location, const FColor& Material, const uint8& Density)
{
	check(0 && "abstract");
	return false;
}

void UVoreealVolume::Internal_SetSize(const FVoreealRegion& Region, bool New)
{
	// Might not be supported
}

void UVoreealVolume::SerializeVolume(FArchive& Ar, FVoreealRegion& Region)
{
	// TODO: Write Version
	Ar << Region;

	FColor Material;
	uint8 Density;
	for (int32 x = Region.X; x < (Region.X + Region.Width); x++)
	{
		for (int32 y = Region.Y; y < (Region.Y + Region.Height); y++)
		{
			for (int32 z = Region.Z; z < (Region.Z + Region.Depth); z++)
			{
				GetVoxel(FVector(x, y, z), Material, Density);
				Ar << Material.DWColor() << Density;
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

	FColor Material;
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
