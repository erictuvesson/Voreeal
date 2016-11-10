#include "VoreealEditorPrivatePCH.h"
#include "VoreealFactory.h"

#include "VoreealBasicVolume.h"

#define LOCTEXT_NAMESPACE "Voreeal"

UVoreealFactory::UVoreealFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBasicVolume::StaticClass();
}

UObject* UVoreealFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UBasicVolume>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE