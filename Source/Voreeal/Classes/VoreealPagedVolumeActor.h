#pragma once

#include "VoreealActor.h"
#include "VoreealPagedVolumeComponent.h"
#include "VoreealPagedVolumeActor.generated.h"

UCLASS(ComponentWrapperClass)
class VOREEAL_API APagedVolumeActor : public AVoreealActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Voreeal", meta = (ExposeFunctionCategories = "Voreeal,Rendering,Physics", AllowPrivateAccess = "true"))
	UPagedVolumeComponent* PagedVolumeComponent;

public:
	APagedVolumeActor(const class FObjectInitializer& ObjectInitializer);

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
	UPagedVolumeComponent* GetPagedVolumeComponent() const;
};
