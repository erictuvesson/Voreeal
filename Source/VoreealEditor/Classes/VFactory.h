#pragma once

#include "UnrealEd.h"
#include "VFactory.generated.h"

/// Factory for Voreeal Volume.
UCLASS()
class UVFactory : public UFactory
{
	GENERATED_BODY()

public:
	UVFactory(const class FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
