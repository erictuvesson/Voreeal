#pragma once

#include "VActor.h"
#include "VRegionVolumeComponent.h"
#include "VRegionVolumeActor.generated.h"

UCLASS(ComponentWrapperClass)
class VOREEAL_API ARegionVolumeActor : public AVActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Voreeal, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	URegionVolumeComponent* RegionVolumeComponent;

public:
	ARegionVolumeActor(const class FObjectInitializer& ObjectInitializer);

	// Begin AActor Interface
#if WITH_EDITOR
	virtual void CheckForErrors() override;
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif // WITH_EDITOR	
	// End AActor Interface

protected:
	// Begin UObject Interface
	virtual FString GetDetailedInfoInternal() const override;
	// End UObject Interface

public:
	URegionVolumeComponent* GetRegionVolumeComponent() const;
};
