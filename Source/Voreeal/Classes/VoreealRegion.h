#pragma once

#include <PolyVox/Region.h>

#include "VoreealRegion.generated.h"

/// Defines a region within a volume.
USTRUCT(BlueprintType)
struct VOREEAL_API FVoreealRegion
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

	FORCEINLINE FVoreealRegion()
		: X(0), Y(0), Z(0), Width(0), Height(0), Depth(0)
	{ }

	FORCEINLINE FVoreealRegion(int32 X, int32 Y, int32 Z, int32 Width, int32 Height, int32 Depth)
		: X(X), Y(Y), Z(Z), Width(Width), Height(Height), Depth(Depth)
	{ }

	explicit FORCEINLINE FVoreealRegion(FVector Upper)
		: FVoreealRegion(FVector::ZeroVector, Upper)
	{ }

	FORCEINLINE FVoreealRegion(FVector Lower, FVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }

	FORCEINLINE FVoreealRegion(FIntVector Lower, FIntVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }

	FORCEINLINE FVoreealRegion(const PolyVox::Region &InRegion)
	{
		FVoreealRegion::operator=(InRegion);
	}
 
	/// Gets the upper corner.
	FVector Min() const { return FVector(X, Y, Z); }

	/// Gets the lower corner.
	FVector Max() const { return FVector(X + Width, Y + Height, Z + Depth); }

	FIntVector GetUpper() const { return FIntVector(X, Y, Z); }
	FIntVector GetLower() const { return FIntVector(X + Width, Y + Height, Z + Depth); }

	/// Gets the size.
	FVector Size() const { return FVector(Width, Height, Depth); }

	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("Min=[%s] Max=[%s]"), *Min().ToString(), *Max().ToString());
	}

	FORCEINLINE FVoreealRegion& operator=(const PolyVox::Region& Other)
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

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FVoreealRegion& Region)
	{
		Ar << Region.X << Region.Y << Region.Z;
		Ar << Region.Width << Region.Height << Region.Depth;
		return Ar;
	}
};
