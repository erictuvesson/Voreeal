#pragma once

#include <PolyVox/MaterialDensityPair.h>

#pragma warning(push)
#pragma warning(disable: 4456)
#include <PolyVox/PagedVolume.h>
#pragma warning(pop) 

#include "VoreealVolume.h"
#include "VoreealPagedVolume.generated.h"

UCLASS(Blueprintable)
class VOREEAL_API UPagedVolumePager : public UObject
{
	GENERATED_BODY()

public:
	// Called when loading chunk, this can either be loading from file or create a procedural world.
	UFUNCTION(BlueprintNativeEvent, Category = "Voreeal")
	void OnLoadChunk(UVoreealPagedVolume* Volume, FVoreealRegion Region);
	virtual void OnLoadChunk_Implementation(UVoreealPagedVolume* Volume, FVoreealRegion Region);

	// Called when unloading a chunk, giving you a chance to save it to a file.
	UFUNCTION(BlueprintNativeEvent, Category = "Voreeal")
	void OnUnloadChunk(UVoreealPagedVolume* Volume, FVoreealRegion Region);
	virtual void OnUnloadChunk_Implementation(UVoreealPagedVolume* Volume, FVoreealRegion Region);
};

UCLASS(Blueprintable)
class VOREEAL_API UVoreealPagedVolume
	: public UVoreealVolume
	, public VoreealExtractHelper<PolyVox::PagedVolume<PolyVox::MaterialDensityPair88>, PolyVox::MaterialDensityPair88>
{
	GENERATED_BODY()

public:
	UVoreealPagedVolume(const class FObjectInitializer& ObjectInitializer);

	// Begin UVoreealVolume Interface
	virtual bool IsValid() const override;
	virtual void GetVoxel(const FVector& Location, uint8& Material, uint8& Density) override;
	virtual FVoreealMesh ExtractMesh(const FVoreealExtractorOptions& Options) override;
	// End UVoreealVolume Interface

	// Add chunk at X,Y,Z.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	bool AddChunk(int32 X, int32 Y, int32 Z);
	
	// Remove chunk at X,Y,Z.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	bool RemoveChunk(int32 X, int32 Y, int32 Z);

	// Remove all the existing chunks.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void RemoveAllChunks();

	// Calculate size in megabytes.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	float CalculateSizeInMegabytes() { return CalculateSizeInBytes() / 1000.0f; }

	// Calculate size in bytes.
	uint32 CalculateSizeInBytes();

protected:
	virtual bool Internal_SetVoxel(FVector Location, const uint8& Material, const uint8& Density) override;

private:
	TSharedPtr<VolumeType> Volume;
};
