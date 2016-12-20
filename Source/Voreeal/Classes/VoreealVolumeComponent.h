#pragma once

#include "VoreealWorker.h"
#include "VoreealVolumeComponent.generated.h"

UENUM(Blueprintable)
enum class EPickVoxelType
{
	// Pick the next voxel we hit, this is mostly used for painting or editing an existing voxel.
	PickFirst,
	// Pick the last voxel, this is mostly used when adding new voxels. 
	PickLast,
};

UCLASS(Abstract)
class VOREEAL_API UVoreealVolumeComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Voreeal")
	class UProceduralMeshComponent* MeshComponent;

public:
	UVoreealVolumeComponent(const class FObjectInitializer& ObjectInitializer);
	
	// Convert from world-space to volume-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void WorldPositionToVolumePosition(const FVector& WorldPosition, FVector& Result) const;

	// Convert from volume-space to world-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void VolumePositionToWorldPosition(const FVector& LocalPosition, FVector& Result) const;

	// Convert from world-space to volume-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void WorldDirectionToVolumeDirection(const FVector& WorldDirection, FVector& Result) const;

	// Convert from volume-space to world-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void VolumeDirectionToWorldDirection(const FVector& LocalDirection, FVector& Result) const;

protected:
	// Get if there are any finished tasks.
	bool FindFinishedTask(TSharedPtr<FVoreealMesh>& Result);

	// Queue a new task.
	void AddTask(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options);

private:
	uint32 Identifier;

	static FVoreealWorker* Runnable;
	static void InitWorker();

	// TODO: Fix
	friend class UVoreealBlueprintLibrary;
};
