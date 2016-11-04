#include "VoreealPrivatePCH.h"
#include "VBaseVolume.h"

void UBaseVolume::SetVoxel(const FVector& Location, const FColor& Color)
{
	// TODO: Check if valid? Currently not doing because of performance.

	uint32 data = 
		(Color.R) | 
		(Color.G) << 8 |
		(Color.B) << 16 |
		(Color.A) << 24;
		
	Internal_SetVoxel(Location, data);
}

void UBaseVolume::GetVoxel(const FVector& Location, FColor& Color)
{
	// TODO: Check if valid? Currently not doing because of performance.

	uint32 data;
	Internal_GetVoxel(Location, data);
	
	Color.R = (data & 0x000000FF);
	Color.G = (data & 0x0000FF00) >> 8;
	Color.B = (data & 0x00FF0000) >> 16;
	Color.A = (data & 0xFF000000) >> 24;
}

bool UBaseVolume::IsValid() const
{
	check(0 && "abstract");
	return false;
}

bool UBaseVolume::Internal_SetVoxel(const FVector& Location, const uint32& Data)
{
	check(0 && "abstract");
	return false;
}

void UBaseVolume::Internal_GetVoxel(const FVector& Location, uint32& Data)
{
	check(0 && "abstract");
}

void UBaseVolume::Internal_SetSize(const FRegion& Region, bool New)
{
	check(0 && "abstract");
}

void UBaseVolume::SerializeVolume(FArchive& Ar, FRegion& Region)
{
	Ar << Region;

	for (int32 x = Region.X; x < (Region.X + Region.Width); x++)
	{
		for (int32 y = Region.Y; y < (Region.Y + Region.Height); y++)
		{
			for (int32 z = Region.Z; z < (Region.Z + Region.Depth); z++)
			{
				uint32 data;
				Internal_GetVoxel(FVector(x, y, z), data);
				Ar << data;
			}
		}
	}
}

void UBaseVolume::DeserializeVolume(FArchive& Ar)
{
	FRegion region;
	Ar << region;

	Internal_SetSize(region, true);

	uint32 data;
	for (int32 x = region.X; x < (region.X + region.Width); x++)
	{
		for (int32 y = region.Y; y < (region.Y + region.Height); y++)
		{
			for (int32 z = region.Z; z < (region.Z + region.Depth); z++)
			{
				Ar << data;
				Internal_SetVoxel(FVector(x, y, z), data);
			}
		}
	}

	OnChanged.Broadcast(region);
}
