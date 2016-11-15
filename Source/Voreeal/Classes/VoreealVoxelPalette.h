#pragma once

#include "VoreealVoxelPalette.generated.h"

USTRUCT(BlueprintType)
struct FVoxelData
{
	GENERATED_BODY();

	FColor Color;
	
};

UCLASS()
class UVoxelPalette : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Voreeal|Palette")
	TMap<uint8, FVoxelData> Palette;

};
