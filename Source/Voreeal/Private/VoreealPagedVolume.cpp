#include "VoreealPrivatePCH.h"
#include "VoreealPagedVolume.h"

void UPagedVolumePager::OnLoadChunk_Implementation(UVoreealPagedVolume* Volume, FVoreealRegion Region)
{
	// Do procedural code here!
}

void UPagedVolumePager::OnUnloadChunk_Implementation(UVoreealPagedVolume* Volume, FVoreealRegion Region)
{
	// Save the world here!
}

UVoreealPagedVolume::UVoreealPagedVolume(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UVoreealPagedVolume::IsValid() const
{
	return Volume.IsValid();
}

void UVoreealPagedVolume::GetVoxel(const FVector& Location, FColor& Material, uint8& Density)
{
	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
	PolyVox::MaterialDensityPair32 Data = Volume->getVoxel(pos);
	Material = FColor(Data.getMaterial());
	Density = Data.getDensity();
}

FVoreealMesh UVoreealPagedVolume::ExtractMesh(const FVoreealExtractorOptions& Options)
{
	return ExtractMeshHelper(Volume.Get(), ExtractorType, Options);
}

bool UVoreealPagedVolume::AddChunk(int32 X, int32 Y, int32 Z)
{
	if (IsValid())
	{
		// TODO: 
		return true;
	}
	return false;
}

bool UVoreealPagedVolume::RemoveChunk(int32 X, int32 Y, int32 Z)
{
	if (IsValid())
	{
		// TODO: 
		return true;
	}
	return false;
}

void UVoreealPagedVolume::RemoveAllChunks()
{
	if (IsValid())
	{
		// TODO:
		//Volume->flush();
	}
}

uint32 UVoreealPagedVolume::CalculateSizeInBytes()
{
	return IsValid() ? Volume->calculateSizeInBytes() : 0;
}

bool UVoreealPagedVolume::Internal_SetVoxel(FVector Location, const FColor& Material, const uint8& Density)
{
	// TODO: Check bounds

	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
	Volume->setVoxel(pos, PolyVox::MaterialDensityPair32(Material.DWColor(), Density));

	OnChanged.Broadcast(FVoreealRegion(Location.X, Location.Y, Location.Z, 1, 1, 1));

	return true;
}
