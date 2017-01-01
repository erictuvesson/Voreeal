#include "VoreealPrivatePCH.h"
#include "VoreealPagedVolume.h"

void UPagedVolumePager::OnLoadChunk_Implementation(UVoreealPagedVolume* Volume, FVoreealRegion Region)
{
	// Do procedural code here!

	for (int32 x = 0; x < Region.Width; x++)
	{
		int32 actualX = Region.GetLower().X + x;
		for (int32 z = 0; z < Region.Depth; z++)
		{
			int32 actualZ = Region.GetLower().Z + z;
			for (int32 y = 0; y < Region.Height; y++)
			{
				int32 actualY = Region.GetLower().Y + y;
				if (actualY < 32)
				{
					Volume->SetVoxelXYZ(actualX, actualY, actualZ, FColor::White, 255);
				}
			}
		}
	}
}

void UPagedVolumePager::OnUnloadChunk_Implementation(UVoreealPagedVolume* Volume, FVoreealRegion Region)
{
	// Save the world here!
}

class EmptyPager : public PolyVox::PagedVolume<PolyVox::MaterialDensityPair32>::Pager
{
public:
	virtual void pageIn(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair32>::Chunk* pChunk) {}
	virtual void pageOut(const PolyVox::Region& region, PolyVox::PagedVolume<PolyVox::MaterialDensityPair32>::Chunk* pChunk) {}
};

UVoreealPagedVolume::UVoreealPagedVolume(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Volume = MakeShareable(new PolyVox::PagedVolume<PolyVox::MaterialDensityPair32>(new EmptyPager()));
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
	if (Volume.IsValid())
	{
		PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
		Volume->setVoxel(pos, PolyVox::MaterialDensityPair32(Material.DWColor(), Density));

		OnChanged.Broadcast(FVoreealRegion(Location.X, Location.Y, Location.Z, 1, 1, 1));
		return true;
	}
	return false;
}
