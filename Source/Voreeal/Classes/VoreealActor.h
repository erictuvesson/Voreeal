#pragma once

#include <GameFramework/Actor.h>

#include "VoreealActor.generated.h"

/** Voreeal Base Actor, this providers easy methods for converting values. */
UCLASS(Abstract, HideCategories = (Input, Collision, Replication))
class VOREEAL_API AVoreealActor : public AActor
{
	GENERATED_BODY()

public:
	AVoreealActor(const class FObjectInitializer& ObjectInitializer);
	
	/// Convert from world-space to volume-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void WorldPositionToVolumePosition(const FVector& WorldPosition, FVector& Result) const;

	/// Convert from volume-space to world-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void VolumePositionToWorldPosition(const FVector& LocalPosition, FVector& Result) const;

	/// Convert from world-space to volume-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void WorldDirectionToVolumeDirection(const FVector& WorldDirection, FVector& Result) const;

	/// Convert from volume-space to world-space
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	void VolumeDirectionToWorldDirection(const FVector& LocalDirection, FVector& Result) const;

public:
	/// Along a raycast, get the position of the first non-empty voxel.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	virtual bool PickFirstSolidVoxel(const FVector& LocalStartPosition, const FVector& LocalDirection, FVector& VoxelLocation) const;

	/// Along a raycast, get the position of the last non-empty voxel.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Voreeal)
	virtual bool PickLastSolidVoxel(const FVector& LocalStartPosition, const FVector& LocalDirection, FVector& VoxelLocation) const;

/*
	/// Create a region actor from this volume.
	UFUNCTION(BlueprintCallable, Category = Voreeal)
	virtual bool CreateRegionActor(const FVoreealRegion& Region, class ARegionVolumeActor* RegionActor);
*/
};
