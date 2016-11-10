#pragma once

#include "UnrealEd.h"
#include "VoreealFactory.generated.h"

/// Factory for Voreeal Volume.
UCLASS()
class UVoreealFactory : public UFactory
{
	GENERATED_BODY()

public:
	UVoreealFactory(const class FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
