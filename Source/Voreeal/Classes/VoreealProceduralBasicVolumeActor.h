#pragma once

#include "VoreealActor.h"
#include "VoreealBasicVolumeComponent.h"
#include "VoreealProceduralBasicVolumeActor.generated.h"

UCLASS()
class VOREEAL_API AProceduralBasicVolumeActor : public AVoreealActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Voreeal")
	UBasicVolumeComponent* BasicVolumeComponent;

public:
	AProceduralBasicVolumeActor(const class FObjectInitializer& ObjectInitializer);

	// Begin AActor Interface
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual bool GetReferencedContentObjects(TArray<UObject*>& Objects) const override;
#endif 	
	// End AActor Interface

};
