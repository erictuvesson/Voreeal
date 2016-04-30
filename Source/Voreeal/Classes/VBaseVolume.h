#pragma once

#include <ProceduralMeshComponent.h>

#include "VRegion.h"

#include "VBaseVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVolumeChanged, FRegion, Region);

USTRUCT(BlueprintType)
struct VOREEAL_API FProceduralMesh
{
	GENERATED_BODY()

	TArray<FVector> Vertices;
	TArray<int32> Indices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FVector> Colors;
	TArray<FProcMeshTangent> Tangents;
};

UENUM(BlueprintType)
enum class EVolumeExtractType : uint8
{
	ColoredCubes,
	MarchingCubes
};

UCLASS(Abstract)
class VOREEAL_API UBaseVolume : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = Voreeal)
	FVolumeChanged OnChanged;

	UPROPERTY(EditDefaultsOnly, Category = Voreeal)
	EVolumeExtractType ExtractorType;

public:

	/// Extract mesh.
	UFUNCTION(BlueprintCallable, Category = Voreeal)
	virtual void ExtractMesh(const FRegion& Region, const int32& LOD, FProceduralMesh& Mesh);

    /// Set Voxel at Location.
	UFUNCTION(BlueprintCallable, Category = Voreeal)
    void SetVoxel(const FVector& Location, const FColor& Color);
    
    /// Get Voxel at Location.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
    void GetVoxel(const FVector& Location, FColor& Color);

	/// Checks if the volume is valid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
    virtual bool IsValid() const;

public:
	void SerializeVolume(FArchive& Ar, FRegion& Region);
	void DeserializeVolume(FArchive& Ar);

protected:
	virtual bool Internal_SetVoxel(const FVector& Location, const uint32& Data);
	virtual void Internal_GetVoxel(const FVector& Location, uint32& Data);
	virtual void Internal_SetSize(const FRegion& Region, bool New);
};
