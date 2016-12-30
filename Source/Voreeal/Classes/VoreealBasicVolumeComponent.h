#pragma once

#include "VoreealOctree.h"
#include "VoreealBasicVolume.h"
#include "VoreealVolumeComponent.h"
#include "VoreealBasicVolumeComponent.generated.h"

/** Basic Voxel Volume Component, a fixed volume. */
UCLASS(ClassGroup = (Rendering, Common), HideCategories = (Object, Activation, "Components|Activation"), EditInlineNew, meta = (BlueprintSpawnableComponent))
class VOREEAL_API UBasicVolumeComponent : public UVoreealVolumeComponent
{
	GENERATED_BODY()

public:
	// Voxel Volume Asset
	DEPRECATED(4.14, "Will be made private. Use GetVolume to get volume and SetBasicVolume to set the volume.")
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voreeal", meta = (DisplayThumbnail = "true"))
	UBasicVolume* Volume;

	// Override the extractor type.
	uint32 bOverrideExtractor : 1;

	// Extractor type to override.
	UPROPERTY(EditDefaultsOnly, Category = "Voreeal")
	EVolumeExtractor ExtractorType;

public:
	UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer);

	// Begin UObject Interface
	virtual void PostLoad() override;
	virtual FString GetDetailedInfoInternal() const override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject Interface

	// Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;
	// End UActorComponent Interface

	// Change the Voxel Volume used by this instance.
	UFUNCTION(BlueprintCallable, Category = "Components|Voreeal")
	virtual bool SetBasicVolume(UBasicVolume* NewVolume);

	// Debug draw the octree of the volume.
	UFUNCTION(BlueprintCallable, Category = "Voreeal|Octree|Debug")
	void DrawDebugOctree(const FLinearColor& Color, float Duration, float Thickness);
	
	// Marks the volume for full redraw.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void MarkVolumeDirty();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	FORCEINLINE bool PickVoxel(const FVector& Start, const FVector& End, EPickVoxelType PickVoxelType, FIntVector& HitPoint) const
	{
		switch (PickVoxelType) {
		default:
		case EPickVoxelType::PickFirst: return PickFirstSolidVoxel(Start, End, HitPoint);
		case EPickVoxelType::PickLast: return PickLastSolidVoxel(Start, End, HitPoint);
		}
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	virtual bool PickFirstSolidVoxel(const FVector& Start, const FVector& End, FIntVector& HitPoint) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Voreeal")
	virtual bool PickLastSolidVoxel(const FVector& Start, const FVector& End, FIntVector& HitPoint) const;

public:
	// Get the Volume used by this instance. 
	UBasicVolume* GetVolume() const
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		return Volume;
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
	}

private:
	UFUNCTION()
	void OnVolumeChanged(FVoreealRegion Region);

	void EnsureRendering();
	
private:
	TSharedPtr<FSparseOctree> m_octree;
};
