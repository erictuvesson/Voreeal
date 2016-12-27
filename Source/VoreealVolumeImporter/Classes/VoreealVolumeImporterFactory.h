#pragma once

#include "UnrealEd.h"
#include "VoreealVolumeImporterFactory.generated.h"

class FVoreealImporter;

UCLASS(hidecategories = Object)
class UVoreealVolumeImporterFactory : public UFactory, public FReimportHandler
{
	GENERATED_BODY()

private:
	TArray<FVoreealImporter*> importers;

public:
	UVoreealVolumeImporterFactory(const class FObjectInitializer& ObjectInitializer);
	virtual ~UVoreealVolumeImporterFactory();

	// Begin UFactory Interface
	virtual FText GetDisplayName() const override;
	virtual bool DoesSupportClass(UClass * Class) override;
	virtual UClass* ResolveSupportedClass() override;
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled);
	// End UFactory Interface

	// Begub FReimportHandler Interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	virtual int32 GetPriority() const override;
	// End FReimportHandler Interface
};
