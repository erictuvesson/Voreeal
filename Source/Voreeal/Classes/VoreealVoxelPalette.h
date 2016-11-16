#pragma once

#include "VoreealVoxelPalette.generated.h"

USTRUCT(BlueprintType)
struct FVoxelPaletteData : public FTableRowBase
{
	GENERATED_BODY()

	FVoxelPaletteData()
		: Color(FColor::Gray)
	{ }

	// The vertex color of the voxel.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voreeal|Palette")
	FColor Color;
};
