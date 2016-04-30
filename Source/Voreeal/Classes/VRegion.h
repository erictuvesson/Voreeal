#pragma once

#include <PolyVox/Region.h>

#include "VRegion.generated.h"

/// Defines a region within a volume.
USTRUCT(BlueprintType)
struct VOREEAL_API FRegion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Depth;

	FORCEINLINE FRegion()
		: X(0), Y(0), Z(0), Width(0), Height(0), Depth(0)
	{ }

	FORCEINLINE FRegion(int32 X, int32 Y, int32 Z, int32 Width, int32 Height, int32 Depth)
		: X(X), Y(Y), Z(Z), Width(Width), Height(Height), Depth(Depth)
	{ }

	explicit FORCEINLINE FRegion(FVector Upper)
		: FRegion(FVector::ZeroVector, Upper)
	{ }

	FORCEINLINE FRegion(FVector Lower, FVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }
    
	FORCEINLINE FRegion(const PolyVox::Region &InRegion)
    {
        FRegion::operator=(InRegion);
    }
 
	FORCEINLINE FRegion& operator=(const PolyVox::Region& Other)
    {
		int32 width = Other.getWidthInVoxels();
		int32 height = Other.getHeightInVoxels();
		int32 depth = Other.getDepthInVoxels();

		this->X = Other.getLowerX();
		this->Y = Other.getLowerY();
		this->Z = Other.getLowerZ();
		this->Width = width;
		this->Height = height;
		this->Depth = depth;
 
        return *this;
    }

	explicit FORCEINLINE operator PolyVox::Region() const
	{ 
		return PolyVox::Region(X, Y, Z, X + Width, Y + Height, Z + Depth);
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FRegion& Region)
	{
		Ar << Region.X << Region.Y << Region.Z;
		Ar << Region.Width << Region.Height << Region.Depth;
		return Ar;
	}
};
