#pragma once

#include "UnrealEd.h"
#include "VoreealVoxelPaletteFactory.generated.h"

UCLASS()
class UVoreealVoxelVolumeFactory : public UFactory
{
	GENERATED_BODY()

public:
	UVoreealVoxelVolumeFactory(const class FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
