#pragma once

#include <PolyVox/Vector.h>

/**
 * Extension to FVector, so we can convert from PolyVox::Vector3DFloat to FVector explicitly.
 */
struct FPolyVoxVector : public FVector
{
	FORCEINLINE FPolyVoxVector()
	{ }
 
	explicit FORCEINLINE FPolyVoxVector(EForceInit E)
		: FVector(E)
	{ }
 
	FORCEINLINE FPolyVoxVector(float InX, float InY, float InZ)
		: FVector(InX, InY, InX)
	{ }
 
	FORCEINLINE FPolyVoxVector(const FVector &InVec)
	{
		FVector::operator=(InVec);
	}
 
	FORCEINLINE FPolyVoxVector(const PolyVox::Vector3DFloat &InVec)
	{
		FPolyVoxVector::operator=(InVec);
	}
 
	FORCEINLINE FVector& operator=(const PolyVox::Vector3DFloat& Other)
	{
		this->X = Other.getX();
		this->Y = Other.getY();
		this->Z = Other.getZ();
 
		DiagnosticCheckNaN();
 
		return *this;
	}

	explicit FORCEINLINE operator PolyVox::Vector3DFloat() const
	{
		return PolyVox::Vector3DFloat(X, Y, Z);
	}
};
