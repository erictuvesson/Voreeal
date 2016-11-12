#pragma once

#include <ProceduralMeshComponent.h>

#include "VoreealRegion.h"
#include "VoreealMeshExtractors.h"
#include "VoreealVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVolumeChanged, FVoreealRegion, Region);

/** Base Voxel Volume, used as a data format. */
UCLASS(Abstract)
class VOREEAL_API UVoreealVolume : public UObject
{
	GENERATED_BODY()

public:
	/// Occures when the volume has changed.
	UPROPERTY(BlueprintAssignable, Category = "Voreeal")
	FVolumeChanged OnChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Voreeal")
	TSubclassOf<UVoreealMeshExtractor> MeshExtractor;

public:
	///// Extract mesh.
	//UFUNCTION(BlueprintCallable, Category = Voreeal)
	//virtual void ExtractMesh(const FVoreealRegion& Region, const int32& LOD, FProceduralMesh& Mesh);

	/// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void SetVoxelXYZ(const int32& X, const int32& Y, const int32& Z, const FColor& Color) {
		SetVoxel(FVector(X, Y, Z), Color);
	}

	/// Set Voxel at Location.
	void SetVoxel(const FIntVector& Location, const FColor& Color) {
		SetVoxel(FVector(Location.X, Location.Y, Location.Z), Color);
	}

	/// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void SetVoxel(const FVector& Location, const FColor& Color);
	
	/// Get Voxel at Location.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	void GetVoxel(const FVector& Location, FColor& Color);

	/// Checks if the volume is valid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	virtual bool IsValid() const;

public:
	void SerializeVolume(FArchive& Ar, FVoreealRegion& Region);
	void DeserializeVolume(FArchive& Ar);

protected:
	virtual bool Internal_SetVoxel(const FVector& Location, const uint32& Data);
	virtual void Internal_GetVoxel(const FVector& Location, uint32& Data);
	virtual void Internal_SetSize(const FVoreealRegion& Region, bool New);
};
