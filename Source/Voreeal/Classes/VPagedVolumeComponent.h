#pragma once

//#include "VComponent.h"
//#include "VPagedVolume.h"
//#include "VPagedVolumeComponent.generated.h"
//
//UCLASS(ClassGroup = (Rendering, Common), HideCategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), EditInlineNew, meta = (BlueprintSpawnableComponent))
//class UPagedVolumeComponent : public UVComponent
//{
//    GENERATED_BODY()
//    
//public:
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Voreeal, meta = (DisplayThumbnail = "true"))
//	UPagedVolume* Volume;
//
//public:
//	UPagedVolumeComponent(const class FObjectInitializer& ObjectInitializer);
//
//	// Begin UObject Interface
//#if WITH_EDITOR
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//#endif // WITH_EDITOR
//	virtual void PostLoad() override;
//	virtual FString GetDetailedInfoInternal() const override;
//	// End UObject Interface
//
//	UFUNCTION(BlueprintCallable, Category = "Components|Voreeal")
//	virtual bool SetPagedVolume(class UPagedVolume* NewVolume);
//
//};
