#pragma once

#include "UnrealEd.h"
#include "VVolumeImporterFactory.generated.h"

UCLASS()
class VOREEALVOLUMEIMPORTER_API UVVolumeImporterFactory : public UFactory, public FReimportHandler
{
	GENERATED_BODY()

public:
	UVVolumeImporterFactory(const class FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual FText GetToolTip() const override;
	virtual bool FactoryCanImport(const FString& Filename) override;
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
	// End of UFactory interface

	// FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	virtual int32 GetPriority() const override;
	// End of FReimportHandler interface
};
