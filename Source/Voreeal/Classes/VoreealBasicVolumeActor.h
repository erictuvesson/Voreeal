#pragma once

#include "VoreealActor.h"
#include "VoreealBasicVolumeComponent.h"
#include "VoreealBasicVolumeActor.generated.h"

UCLASS(ComponentWrapperClass, meta = (ChildCanTick))
class VOREEAL_API ABasicVolumeActor : public AVoreealActor
{
	GENERATED_BODY()

private_subobject:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Voreeal", meta = (ExposeFunctionCategories = "Voreeal,Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	UBasicVolumeComponent* BasicVolumeComponent;

public:
	ABasicVolumeActor(const class FObjectInitializer& ObjectInitializer);

	// Begin AActor Interface
#if WITH_EDITOR
	virtual void CheckForErrors() override;
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif 	
	// End AActor Interface

protected:
	// Begin UObject Interface
	virtual FString GetDetailedInfoInternal() const override;
	// End UObject Interface

public:
	UBasicVolumeComponent* GetBasicVolumeComponent() const;
};
