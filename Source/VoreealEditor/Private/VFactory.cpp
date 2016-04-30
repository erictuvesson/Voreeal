#include "VoreealEditorPrivatePCH.h"
#include "VFactory.h"

#include "VBasicVolume.h"

#define LOCTEXT_NAMESPACE "Voreeal"

UVFactory::UVFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBasicVolume::StaticClass();
}

UObject* UVFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UBasicVolume>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE