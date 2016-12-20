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

	// Begin AActor Interface
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	// End AActor Interface

/*
	/// Create a region actor from this volume.
	UFUNCTION(BlueprintCallable, Category = Voreeal)
	virtual bool CreateRegionActor(const FVoreealRegion& Region, class ARegionVolumeActor* RegionActor);
*/
};
