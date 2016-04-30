#pragma once

#include "VActorFactory.generated.h"

UCLASS()
class UVActorFactory : public UActorFactory
{
	GENERATED_BODY()
		
public:
	UVActorFactory(const class FObjectInitializer& ObjectInitializer);

	// Begin UActorFactory Interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	// End UActorFactory Interface
};
