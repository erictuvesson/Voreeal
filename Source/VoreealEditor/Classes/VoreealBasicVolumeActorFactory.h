#pragma once

#include "VoreealBasicVolumeActorFactory.generated.h"

UCLASS()
class UVoreealBasicVolumeActorFactory : public UActorFactory
{
	GENERATED_BODY()
		
public:
	UVoreealBasicVolumeActorFactory(const class FObjectInitializer& ObjectInitializer);

	// Begin UActorFactory Interface
	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;
	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;
	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;
	// End UActorFactory Interface
};
