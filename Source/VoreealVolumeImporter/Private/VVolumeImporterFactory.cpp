#include "VoreealVolumeImporterPrivatePCH.h"
#include "VVolumeImporterFactory.h"

#include "VImporters.h"
#include "VBasicVolume.h"

#include <AssetToolsModule.h>
#include <AssetRegistryModule.h>
#include <PackageTools.h>
#include <StringConv.h>

#define LOCTEXT_NAMESPACE "VoreealEditor"

UVVolumeImporterFactory::UVVolumeImporterFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
	//bEditAfterNew = true;
	SupportedClass = UBasicVolume::StaticClass();

	bEditorImport = true;
	bText = true;

	Formats.Add(TEXT("qb;Qubicle Binary"));
	Formats.Add(TEXT("vox;MagicaVoxel"));
	//Formats.Add(TEXT("vox;Voxlap"));
	//Formats.Add(TEXT("kv6;Voxlap"));
	//Formats.Add(TEXT("3mp;Paint3D"));
}

FText UVVolumeImporterFactory::GetToolTip() const
{
	return LOCTEXT("UVVolumeImporterFactoryDescription", "Voreeal Volumes");
}

bool UVVolumeImporterFactory::FactoryCanImport(const FString& Filename)
{
	return Voreeal::Import(Filename) != nullptr;
}

UObject* UVVolumeImporterFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	Flags |= RF_Transactional;

	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	UBasicVolume* Result = NewObject<UBasicVolume>(InParent, InName, Flags);

	auto polyVoxVolume = Voreeal::Import(UFactory::GetCurrentFilename());

	Result->Volume.reset(polyVoxVolume);

	FBufferArchive Ar;
	// TODO: !!!
	//Voreeal::Export(polyVoxVolume, Ar);
	Result->ImportedData = Ar;

	Result->PostEditChange();

	// Store the current file path and timestamp for re-import purposes
	Result->AssetImportData->Update(CurrentFilename);
	
	FEditorDelegates::OnAssetPostImport.Broadcast(this, Result);

	return Result;
}

bool UVVolumeImporterFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	if (UBasicVolume* volume = Cast<UBasicVolume>(Obj))
	{
		if (volume->AssetImportData != nullptr)
		{
			volume->AssetImportData->ExtractFilenames(OutFilenames);
		}
		else
		{
			OutFilenames.Add(FString());
		}
		return true;
	}
	return false;
}

void UVVolumeImporterFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (UBasicVolume* volume = Cast<UBasicVolume>(Obj))
	{
		if (ensure(NewReimportPaths.Num() == 1))
		{
			volume->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
		}
	}
}

EReimportResult::Type UVVolumeImporterFactory::Reimport(UObject* Obj)
{
	if (UBasicVolume* volume = Cast<UBasicVolume>(Obj))
	{
		auto Filename = volume->AssetImportData->GetFirstFilename();

		auto polyVoxVolume = Voreeal::Import(Filename);

		volume->Volume.reset(polyVoxVolume);

		FMemoryWriter MemoryWriter{ volume->ImportedData, true };
		FArchive Ar{ MemoryWriter };
		// TODO: !!!
		//Voreeal::Export(polyVoxVolume, Ar);

		volume->PostEditChange();

		return EReimportResult::Succeeded;
	}
	return EReimportResult::Failed;
}

int32 UVVolumeImporterFactory::GetPriority() const
{
	return ImportPriority;
}

#undef LOCTEXT_NAMESPACE
