#pragma once

#include <ProceduralMeshComponent.h>
#include <PolyVox/MaterialDensityPair.h>

#include "VoreealRegion.h"
#include "VoreealExtractHelper.h"
#include "VoreealVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVolumeChanged, FVoreealRegion, Region);

namespace PolyVox
{
	typedef MaterialDensityPair<uint32, 24, 8> MaterialDensityPair32;
}

/** Base Voxel Volume, used as a data format. */
UCLASS(Abstract)
class VOREEAL_API UVoreealVolume : public UObject
{
	GENERATED_BODY()

public:
	// Occures when the volume has changed.
	UPROPERTY(BlueprintAssignable, Category = "Voreeal")
	FVolumeChanged OnChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Voreeal")
	EVolumeExtractor ExtractorType;

public:
	// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void SetVoxelXYZ(int32 X, int32 Y, int32 Z, const FColor& Material, const uint8& Density) {
		SetVoxel(FVector(X, Y, Z), Material, Density);
	}

	// Set Voxel at Location.
	void SetVoxel(FIntVector Location, const FColor& Material, const uint8& Density) {
		SetVoxel(FVector(Location.X, Location.Y, Location.Z), Material, Density);
	}

	// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void SetVoxel(FVector Location, const FColor& Material, const uint8& Density);
	
	// Get Voxel at Location.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	virtual void GetVoxel(const FVector& Location, FColor& Material, uint8& Density) {}

	// Checks if the volume is valid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	virtual bool IsValid() const;

	// Extract part of the volume.
	virtual FVoreealMesh ExtractMesh(const FVoreealExtractorOptions& Options);

public:
	void SerializeVolume(FArchive& Ar, FVoreealRegion& Region);
	void DeserializeVolume(FArchive& Ar);

protected:
	virtual bool Internal_SetVoxel(FVector Location, const FColor& Material, const uint8& Density);
	virtual void Internal_SetSize(const FVoreealRegion& Region, bool New);
};
