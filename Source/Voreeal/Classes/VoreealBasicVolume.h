#pragma once

#include <PolyVox/RawVolume.h>

#include "VoreealVolume.h"
#include "VoreealBasicVolume.generated.h"

/**
 * Basic Voxel Volume; Enclosed volume with just basic LOD.
 */
UCLASS(Blueprintable, meta = (DisplayThumbnail = "true"))
class VOREEAL_API UBasicVolume 
	: public UVoreealVolume
	, public VoreealExtractHelper<PolyVox::RawVolume<uint32>, uint32>
{
	GENERATED_BODY()
	
public:
	UBasicVolume(const class FObjectInitializer& ObjectInitializer);

	// Begin UObject Interface.
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
#if WITH_EDITORONLY_DATA
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
#endif
	// End UObject Interface

	// Begin UVoreealVolume Interface
	virtual bool IsValid() const override;
	virtual FVoreealMesh ExtractMesh(const FVoreealExtractorOptions& Options) override;
	// End UVoreealVolume Interface
	
	// Gets the enclosing region.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	FVoreealRegion GetEnclosingRegion() const;

	// Resize the volume.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void Resize(const FIntVector& NewSize);

	// Resize the volume.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void ResizeRegion(const FVoreealRegion& NewRegion);

public:
#if WITH_EDITORONLY_DATA
	//UPROPERTY(Category = ImportSettings, VisibleAnywhere, Instanced)
	//class UAssetImportData* AssetImportData;
	class USceneThumbnailInfo* ThumbnailInfo;
#endif

protected:
	virtual bool Internal_SetVoxel(const FVector& Location, const uint32& Data) override;
	virtual void Internal_GetVoxel(const FVector& Location, uint32& Data) override;
	virtual void Internal_SetSize(const FVoreealRegion& Region, bool New) override;

private:
	std::unique_ptr<VolumeType> Volume; // TODO: Switch to UE Ptr

	UPROPERTY()
	TArray<uint8> ImportedData;

	// UVoreealVolumeImporterFactory has to access ImportedData
	friend class UVoreealVolumeImporterFactory;
};
