#pragma once

#include "VComponent.h"
#include "VBaseVolume.h"
#include "VRegionVolumeComponent.generated.h"

UCLASS(ClassGroup = (Rendering, Common), HideCategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), EditInlineNew, meta = (BlueprintSpawnableComponent))
class URegionVolumeComponent : public UVComponent
{
    GENERATED_BODY()
    
	UPROPERTY(VisibleAnywhere, Category = Voreeal)
	class UProceduralMeshComponent* MeshComponent;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Voreeal, meta = (DisplayThumbnail = "true"))
	UBaseVolume* Volume;

public:
	URegionVolumeComponent(const class FObjectInitializer& ObjectInitializer);

	// Begin UObject Interface
	virtual void PostLoad() override;
	virtual FString GetDetailedInfoInternal() const override;
	// End UObject Interface

	UFUNCTION(BlueprintCallable, Category = "Components|Voreeal")
	virtual bool SetVolume(UBaseVolume* NewVolume);

};
