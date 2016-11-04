#pragma once

#include "VRegion.h"
#include "VMeshExtractors.generated.h"

class UBaseVolume;

// TODO: LOD

UCLASS(Abstract)
class UMeshExtractor : public UObject
{
	GENERATED_BODY()

public:
	virtual void ExtractMesh(UBaseVolume* Volume, const FRegion& region, FProcMeshSection& OutSection) { }
};

UCLASS()
class UCubicSurfaceExtractor : public UMeshExtractor
{
	GENERATED_BODY()

protected:
	virtual void ExtractMesh(UBaseVolume* Volume, const FRegion& region, FProcMeshSection& OutSection)  override;
};

UCLASS()
class UMarchingCubesSurfaceExtractor : public UMeshExtractor
{
	GENERATED_BODY()

protected:
	virtual void ExtractMesh(UBaseVolume* Volume, const FRegion& region, FProcMeshSection& OutSection) override;
};
