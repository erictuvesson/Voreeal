#pragma once

#include "VoreealOctree.h"
#include "VoreealBasicVolume.h"
#include "VoreealVolumeComponent.h"
#include "VoreealBasicVolumeComponent.generated.h"

/** Basic Voxel Volume Component, a fixed volume. */
UCLASS(ClassGroup = (Rendering, Common), HideCategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), EditInlineNew, meta = (BlueprintSpawnableComponent))
class UBasicVolumeComponent : public UVoreealVolumeComponent
{
	GENERATED_BODY()

public:
	/// Procedural Voxel Mesh
	UPROPERTY(VisibleAnywhere, Category = "Voreeal")
	class UProceduralMeshComponent* MeshComponent;

	/// Voxel Volume Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voreeal", meta = (DisplayThumbnail = "true"))
	UBasicVolume* Volume;

public:
	UBasicVolumeComponent(const class FObjectInitializer& ObjectInitializer);
	virtual ~UBasicVolumeComponent();

	// Begin UObject Interface
	virtual void PostLoad() override;
	virtual FString GetDetailedInfoInternal() const override;
	// End UObject Interface

	UFUNCTION(BlueprintCallable, Category = "Components|Voreeal")
	virtual bool SetBasicVolume(class UBasicVolume* NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Voreeal|Octree|Debug")
	void DrawDebugOctree(const FColor& Color, float Duration, float Thickness);

private:
	void EnsureRendering();
	void Update();

private:
	FSparseOctree* m_octree;
	FTimerHandle timerHandle;
};
