#pragma once

#include "VoreealOctree.h"
#include "VoreealPagedVolume.h"
#include "VoreealVolumeComponent.h"
#include "VoreealPagedVolumeComponent.generated.h"

class UProceduralMeshComponent;

struct FPagedVolumeChunk
{
	// hashed location used for the hashmap as a key in PagedVolumeComponent.
	uint32 m_locationHash;

	// the bounds of this chunk.
	FVoreealRegion m_bounds;

	// the chunks octree.
	TSharedPtr<FSparseOctree> m_octree;

	FPagedVolumeChunk(TSharedPtr<FSparseOctree> Octree, uint32 LocationHash, FVoreealRegion Bounds);
};

UCLASS(ClassGroup = (Rendering, Common), HideCategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), EditInlineNew, meta = (BlueprintSpawnableComponent))
class VOREEAL_API UPagedVolumeComponent : public UVoreealVolumeComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "Voreeal")
	UVoreealPagedVolume* Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voreeal")
	UPagedVolumePager* Pager;

	UPROPERTY(VisibleAnywhere, Category = "Voreeal")
	TMap<uint32, UProceduralMeshComponent*> ProceduralMeshComponents;

public:
	UPagedVolumeComponent(const class FObjectInitializer& ObjectInitializer);

	// Begin UObject Interface
	virtual void PostLoad() override;
	virtual FString GetDetailedInfoInternal() const override;
	// End UObject Interface

	// Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;
	// End UActorComponent Interface

	UFUNCTION(BlueprintCallable, Category = "Components|Voreeal")
	virtual bool SetPagedVolume(UVoreealPagedVolume* NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Voreeal|TEST")
	void TestChunk(int32 X, int32 Y, int32 Z)
	{
		CreateChunk(X, Y, Z);
	}

protected:
	// Gets the chunk at x,y,z.
	FPagedVolumeChunk* GetChunk(int32 X, int32 Y, int32 Z) const;

	// Create a new chunk.
	FPagedVolumeChunk* CreateChunk(int32 X, int32 Y, int32 Z);

private:
	UFUNCTION()
	void OnVolumeChanged(FVoreealRegion Region);

private:
	TArray<TSharedPtr<FPagedVolumeChunk>> ArrayChunks;
	mutable FPagedVolumeChunk *getChunkCache, *onVolumeCache;
};
