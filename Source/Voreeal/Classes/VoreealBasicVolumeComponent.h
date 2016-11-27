#pragma once

#include "VoreealOctree.h"
#include "VoreealBasicVolume.h"
#include "VoreealVolumeComponent.h"
#include "VoreealBasicVolumeComponent.generated.h"

/** Basic Voxel Volume Component, a fixed volume. */
UCLASS(ClassGroup = (Rendering, Common), HideCategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), EditInlineNew, meta = (BlueprintSpawnableComponent))
class VOREEAL_API UBasicVolumeComponent : public UVoreealVolumeComponent
{
	GENERATED_BODY()

public:
	// Voxel Volume Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "Voreeal", meta = (DisplayThumbnail = "true"))
	UBasicVolume* Volume;

public:
	UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer);
	virtual ~UBasicVolumeComponent();

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

	UFUNCTION(BlueprintCallable, Category = "Components|Voreeal")
	virtual bool SetBasicVolume(UBasicVolume* NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Voreeal|Octree|Debug")
	void DrawDebugOctree(const FLinearColor& Color, float Duration, float Thickness);
	
	// Marks the volume for full redraw.
	UFUNCTION(BlueprintCallable, Category = "Voreeal")
	void MarkVolumeDirty();

private:
	UFUNCTION()
	void OnVolumeChanged(FVoreealRegion Region);

	void EnsureRendering();

private:
	FSparseOctree* m_octree;
};
