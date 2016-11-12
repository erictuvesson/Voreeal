#pragma once

#include <PolyVox/Region.h>

#include "VoreealRegion.generated.h"

/// Defines a region within a volume.
USTRUCT(BlueprintType)
struct VOREEAL_API FVoreealRegion
{
	GENERATED_BODY()

	// Position X component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	// Position Y component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	// Position Z component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Z;

	// Position Width component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	// Position Height component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	// Position Depth component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Depth;

	// Default constructor
	FORCEINLINE FVoreealRegion()
		: X(0), Y(0), Z(0), Width(0), Height(0), Depth(0)
	{ }

	FORCEINLINE FVoreealRegion(int32 X, int32 Y, int32 Z, int32 Width, int32 Height, int32 Depth)
		: X(X), Y(Y), Z(Z), Width(Width), Height(Height), Depth(Depth)
	{ }

	explicit FORCEINLINE FVoreealRegion(FVector Size)
		: FVoreealRegion(FVector::ZeroVector, Size)
	{ }

	FORCEINLINE FVoreealRegion(FVector Lower, FVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }

	FORCEINLINE FVoreealRegion(FIntVector Lower, FIntVector Upper)
		: X(Lower.X), Y(Lower.Y), Z(Lower.Z)
		, Width(Lower.X - Upper.X), Height(Lower.Y - Upper.Y), Depth(Lower.Z - Upper.Z)
	{ }

	// Convert PolyVox::Region to FVoreealRegion.
	FORCEINLINE FVoreealRegion(const PolyVox::Region &InRegion)
	{
		FVoreealRegion::operator=(InRegion);
	}
 
	// Gets the upper corner.
	FVector Min() const { return FVector(X, Y, Z); }

	// Gets the lower corner.
	FVector Max() const { return FVector(X + Width, Y + Height, Z + Depth); }

	FIntVector GetUpperInt() const;
	FIntVector GetLowerInt() const;

	FVector GetUpper() const; 
	FVector GetLower() const;

	// Gets the size.
	FVector Size() const { return FVector(Width, Height, Depth); }

	// Gets the center of the region.
	FVector GetCenter() const;

	// Grow the region.
	void Grow(int32 Width, int32 Height, int32 Depth);

	// Grow the region.
	void Grow(int32 Width, int32 Height, int32 Depth, FVoreealRegion& Result);

	// Grow the region from every direction.
	void GrowUnified(int32 Amount);

	// Grow the region from every direction.
	void GrowUnified(int32 Amount, FVoreealRegion& Result);

	// Shift the upper corner of the region.
	void ShiftUpperCorner(int32 X, int32 Y, int32 Z);

	// Shift the lower corner of the region.
	void ShiftLowerCorner(int32 X, int32 Y, int32 Z);

	// Does this region contain another region.
	static bool Contains(const FVoreealRegion& Region1, const FVoreealRegion& Region2);

	// Does this region intersect another region.
	static bool Intersect(const FVoreealRegion& Region1, const FVoreealRegion& Region2);

	// Region to string.
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

FORCEINLINE FVector FVoreealRegion::GetUpper() const
{
	return FVector(X, Y, Z);
}

FORCEINLINE FVector FVoreealRegion::GetLower() const
{
	return FVector(X + Width, Y + Height, Z + Depth);
}

FORCEINLINE FIntVector FVoreealRegion::GetUpperInt() const
{ 
	return FIntVector(X, Y, Z); 
}

FORCEINLINE FIntVector FVoreealRegion::GetLowerInt() const
{ 
	return FIntVector(X + Width, Y + Height, Z + Depth); 
}

FORCEINLINE FVector FVoreealRegion::GetCenter() const
{
	return FVector(X + (Width / 2), Y + (Height / 2), Z + (Depth / 2));
}

FORCEINLINE void Grow(int32 Width, int32 Height, int32 Depth, FVoreealRegion& Result)
{
	Result = FVoreealRegion();
	Result.X -= Width;
	Result.Y -= Height;
	Result.Z -= Depth;
	Result.Width += Width * 2;
	Result.Height += Height * 2;
	Result.Depth += Depth * 2;
}

FORCEINLINE void FVoreealRegion::Grow(int32 Width, int32 Height, int32 Depth)
{
	X -= Width;
	Y -= Height;
	Z -= Depth;
	Width += Width * 2;
	Height += Height * 2;
	Depth += Depth * 2;
}

FORCEINLINE void FVoreealRegion::GrowUnified(int32 Amount)
{
	X -= Amount;
	Y -= Amount;
	Z -= Amount;
	Width += Amount * 2;
	Height += Amount * 2;
	Depth += Amount * 2;
}

FORCEINLINE void FVoreealRegion::GrowUnified(int32 Amount, FVoreealRegion& Result)
{
	Result = FVoreealRegion();
	Result.X -= Amount;
	Result.Y -= Amount;
	Result.Z -= Amount;
	Result.Width += Amount * 2;
	Result.Height += Amount * 2;
	Result.Depth += Amount * 2;
}

FORCEINLINE void FVoreealRegion::ShiftUpperCorner(int32 X, int32 Y, int32 Z)
{
	X += X;
	Y += Y;
	Z += Z;
	Width -= X;
	Height -= Y;
	Depth -= Z;
}

FORCEINLINE void FVoreealRegion::ShiftUpperCorner(int32 X, int32 Y, int32 Z)
{
	Width += X;
	Height += Y;
	Depth += Z;
}

FORCEINLINE bool FVoreealRegion::Contains(const FVoreealRegion& Region1, const FVoreealRegion& Region2)
{
	return (Region2.X < (Region1.X + Region1.Width))
		&& (Region1.X < (Region2.X + Region2.Width))
		&& (Region2.Y < (Region1.Y + Region1.Height))
		&& (Region1.Y < (Region2.Y + Region2.Height))
		&& (Region2.Z < (Region1.Z + Region1.Depth))
		&& (Region1.Z < (Region2.Z + Region2.Depth));
}

FORCEINLINE bool FVoreealRegion::Intersect(const FVoreealRegion& Region1, const FVoreealRegion& Region2)
{
	// TODO: Intersect
	return (Region2.X < (Region1.X + Region1.Width))
		&& (Region1.X < (Region2.X + Region2.Width))
		&& (Region2.Y < (Region1.Y + Region1.Height))
		&& (Region1.Y < (Region2.Y + Region2.Height))
		&& (Region2.Z < (Region1.Z + Region1.Depth))
		&& (Region1.Z < (Region2.Z + Region2.Depth));
}
