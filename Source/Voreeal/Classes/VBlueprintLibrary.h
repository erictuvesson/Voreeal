#pragma once

#include "VRegion.h"
#include "VBlueprintLibrary.generated.h"

UCLASS()
class UVBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public: // Region Methods

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetCenter(const FRegion& Region, FVector& Result);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetUpperCorner(const FRegion& Region, FVector& Result);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GetLowerCorner(const FRegion& Region, FVector& Result);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void GrowUnified(const FRegion& Region, int32 Amount, FRegion& Result) { Grow(Region, Amount, Amount, Amount, Result); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void Grow(const FRegion& Region, int32 Width, int32 Height, int32 Depth, FRegion& Result);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void ShiftUpperCorner(const FRegion& Region, int32 X, int32 Y, int32 Z, FRegion& Result);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static void ShiftLowerCorner(const FRegion& Region, int32 X, int32 Y, int32 Z, FRegion& Result);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal|Region")
	static bool Intersect(const FRegion& Region1, const FRegion& Region2);

public:

	//static void DebugDrawRegion();

};
