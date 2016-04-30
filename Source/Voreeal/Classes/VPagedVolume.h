#pragma once

//// error : Class 'UPagedVolume' contains a dependency (#include or DependsOn) to itself
////		   I get this error because I include PagedVolume from PolyVox!
//#include <PolyVox/PagedVolume.h>
//
//#include "VBaseVolume.h"
//
//#include "VPagedVolume.generated.h"
//
//UCLASS(Blueprintable)
//class VOREEAL_API UPagedVolume : public UBaseVolume
//{
//    GENERATED_BODY()
//    
//public:
//    
//    typedef PolyVox::PagedVolume<uint32> VolumeType;
//    
//public:
//	UPagedVolume(const class FObjectInitializer& ObjectInitializer);
//
//	// Begin UObject Interface.
//	virtual void Serialize(FArchive& Ar) override;
//	virtual void PostInitProperties() override;
//	virtual void PostLoad() override;
//#if WITH_EDITORONLY_DATA
//	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
//#endif
//	// End UObject Interface
//
//    // Begin UBaseVolume Interface
//	virtual void ExtractMesh(const FRegion& Region, const int32& LOD, FProceduralMesh& Mesh) override;
//    virtual bool IsValid() const override;
//    // End UBaseVolume Interface
//
//    /// Add chunk at X,Y,Z.
//	UFUNCTION(BlueprintCallable, Category = Voreeal)
//    bool AddChunk(int32 X, int32 Y, int32 Z);
//    
//    /// Remove chunk at X,Y,Z.
//	UFUNCTION(BlueprintCallable, Category = Voreeal)
//    bool RemoveChunk(int32 X, int32 Y, int32 Z);
//
//    /// Remove all the existing chunks.
//	UFUNCTION(BlueprintCallable, Category = Voreeal)
//    void RemoveAllChunks();
//
//	/// Calculate size in megabytes.
//	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
//	float CalculateSizeInMegabytes() { return CalculateSizeInBytes() / 1000.0f; }
//
//    /// Calculate size in bytes.
//    uint32 CalculateSizeInBytes();
//
//protected:
//	virtual bool Internal_SetVoxel(const FVector& Location, const uint32& Data) override;
//	virtual void Internal_GetVoxel(const FVector& Location, uint32& Data) override;
//
//private:
//    std::unique_ptr<VolumeType> Volume;
//};
