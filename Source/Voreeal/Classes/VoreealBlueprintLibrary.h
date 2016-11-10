#pragma once

#include "VoreealRegion.h"
#include "VoreealBlueprintLibrary.generated.h"

UCLASS()
class UVoreealBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: // Region Methods

	/// Gets the center of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetCenter(const FVoreealRegion& Region, FVector& Result);

	/// Gets the upper corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetUpperCorner(const FVoreealRegion& Region, FVector& Result);

	/// Gets the lower corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetLowerCorner(const FVoreealRegion& Region, FVector& Result);

	/// Grow the region from every direction.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GrowUnified(const FVoreealRegion& Region, int32 Amount, FVoreealRegion& Result) { Grow(Region, Amount, Amount, Amount, Result); }

	/// Grow the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void Grow(const FVoreealRegion& Region, int32 Width, int32 Height, int32 Depth, FVoreealRegion& Result);

	/// Shift the upper corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void ShiftUpperCorner(const FVoreealRegion& Region, int32 X, int32 Y, int32 Z, FVoreealRegion& Result);

	/// Shift the lower corner of the region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void ShiftLowerCorner(const FVoreealRegion& Region, int32 X, int32 Y, int32 Z, FVoreealRegion& Result);

	/// Region intersect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static bool Contains(const FVoreealRegion& Region1, const FVoreealRegion& Region2);

	/// Region intersect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static bool Intersect(const FVoreealRegion& Region1, const FVoreealRegion& Region2);

public:

	/// Draw Debug Region, 
	UFUNCTION(BlueprintCallable, Category = "Voreeal|Region|Debug", meta = (WorldContext = "WorldContextObject"))
	static void DrawDebugRegion(UObject* WorldContextObject, const FTransform& Transform, const FVoreealRegion& Region,
		const FColor& Color, float Duration, float Thickness);

};
