#pragma once

#include "VRegion.h"
#include "VBlueprintLibrary.generated.h"

UCLASS()
class UVBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: // Region Methods

	/// Gets the center of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetCenter(const FRegion& Region, FVector& Result);

	/// Gets the upper corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetUpperCorner(const FRegion& Region, FVector& Result);

	/// Gets the lower corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetLowerCorner(const FRegion& Region, FVector& Result);

	/// Grow the region from every direction.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GrowUnified(const FRegion& Region, int32 Amount, FRegion& Result) { Grow(Region, Amount, Amount, Amount, Result); }

	/// Grow the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void Grow(const FRegion& Region, int32 Width, int32 Height, int32 Depth, FRegion& Result);

	/// Shift the upper corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void ShiftUpperCorner(const FRegion& Region, int32 X, int32 Y, int32 Z, FRegion& Result);

	/// Shift the lower corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void ShiftLowerCorner(const FRegion& Region, int32 X, int32 Y, int32 Z, FRegion& Result);

	/// Region intersect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static bool Contains(const FRegion& Region1, const FRegion& Region2);

	/// Region intersect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static bool Intersect(const FRegion& Region1, const FRegion& Region2);

public:

	/// Draw Debug Region, 
	UFUNCTION(BlueprintCallable, Category = "Voreeal|Region|Debug", meta = (WorldContext = "WorldContextObject"))
	static void DrawDebugRegion(UObject* WorldContextObject, const FTransform& Transform, const FRegion& Region,
		const FColor& Color, float Duration, float Thickness);

};
