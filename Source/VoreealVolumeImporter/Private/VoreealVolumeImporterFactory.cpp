#include "VoreealVolumeImporterPrivatePCH.h"
#include "VoreealVolumeImporterFactory.h"

#include "VoreealBasicVolume.h"

#include "VoreealImporter.h"

#include "Formats/MagicaVoxelImporter.h"
#include "Formats/QubicleBinaryImporter.h"


#include <AssetToolsModule.h>
#include <AssetRegistryModule.h>
#include <PackageTools.h>
#include <StringConv.h>

#define LOCTEXT_NAMESPACE "VoreealEditor"

UVoreealVolumeImporterFactory::UVoreealVolumeImporterFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
	bEditAfterNew = false;
	SupportedClass = UBasicVolume::StaticClass();

	bEditorImport = true;
	bText = false;

	//IVoreealImporterModule& VoreealImporter = IVoreealImporterModule::Get();

	Formats.Add("vox;MagicaVoxel/Voxlap");
	Formats.Add("qb;Qubicle Binary");

	importers.Add(new FMagicaVoxelImporter());
	importers.Add(new FQubicleBinaryImporter());
}

UVoreealVolumeImporterFactory::~UVoreealVolumeImporterFactory()
{
	for (int32 i = 0; i < importers.Num(); i++)
		delete importers[i];
	importers.Empty();
}

FText UVoreealVolumeImporterFactory::GetDisplayName() const
{
	return LOCTEXT("VoreealVolumeImporterFactoryDescription", "Voreeal Volumes");
}

bool UVoreealVolumeImporterFactory::DoesSupportClass(UClass * Class)
{
	return Class == UBasicVolume::StaticClass();
}

UClass* UVoreealVolumeImporterFactory::ResolveSupportedClass()
{
	return UBasicVolume::StaticClass();
}

UObject* UVoreealVolumeImporterFactory::FactoryCreateFile(UClass* InClass, UObject* InParent,
	FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Params, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Params);

	// Load file into byte array
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *CurrentFilename))
	{
		UE_LOG(LogVoreealImporter, Error, TEXT("Invalid File"));
		return nullptr;
	}

	// Create the volume we are importing
	UBasicVolume* Result = NewObject<UBasicVolume>(InParent, InName, Flags);

	FString FilenameExtension = FPaths::GetExtension(CurrentFilename);

	// Loop all the importers
	//IVoreealImporterModule& VoreealImporter = IVoreealImporterModule::Get();
	//auto Importers = VoreealImporter.GetImporters();
	for (auto Importer : importers)
	{
		// Check if the format matches the file extension
		if (Importer->GetExtension() == FilenameExtension)
		{
			FMemoryReader Ar = FMemoryReader(TheBinaryArray, true);
			if (!Importer->Import(Ar, Result, Warn))
			{
				delete Result;
				Result = nullptr;
				return nullptr;
			}
		}
	}

	FVoreealRegion Region = Result->GetEnclosingRegion();
	FBufferArchive Ar;
	Result->SerializeVolume(Ar, Region);
	Result->ImportedData = Ar;

	Result->PostEditChange();

	// Store the current file path and timestamp for re-import purposes
	Result->AssetImportData->Update(CurrentFilename);
	
	FEditorDelegates::OnAssetPostImport.Broadcast(this, Result);

	return Result;
}

bool UVoreealVolumeImporterFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UAssetImportData* ImportData = nullptr;
	if (Obj->GetClass() == UBasicVolume::StaticClass())
	{
		// TODO: 
	}

	if (ImportData)
	{
		FString FileExt = FPaths::GetExtension(ImportData->GetFirstFilename()).ToLower();
		if (FileExt == "vox" || FileExt == "qb") //< TODO: 
		{
			ImportData->ExtractFilenames(OutFilenames);
			return true;
		}
	}
	return false;
}

void UVoreealVolumeImporterFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UBasicVolume* Volume = Cast<UBasicVolume>(Obj);
	if (Volume && Volume->AssetImportData && ensure(NewReimportPaths.Num() == 1))
	{
		Volume->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UVoreealVolumeImporterFactory::Reimport(UObject* Obj)
{
	//ImportSettings->bReimport = true;
	if (Obj->GetClass() == UBasicVolume::StaticClass())
	{
		/*UBasicVolume* Volume = Cast<UBasicVolume>(Obj);
		if (!Volume)
		{
			return EReimportResult::Failed;
		}

		CurrentFilename = Volume->AssetImportData->GetFirstFilename();

		EReimportResult::Type Result = ReimportBasicVolume(Volume);

		if (Volume->GetOuter())
		{
			Volume->GetOuter()->MarkPackageDirty();
		}
		else
		{
			Volume->MarkPackageDirty();
		}

		return Result;*/
	}

	return EReimportResult::Failed;
}

int32 UVoreealVolumeImporterFactory::GetPriority() const
{
	return ImportPriority;
}

#undef LOCTEXT_NAMESPACE
