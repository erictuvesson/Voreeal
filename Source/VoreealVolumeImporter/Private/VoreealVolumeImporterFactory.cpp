#include "VoreealVolumeImporterPrivatePCH.h"
#include "VoreealVolumeImporterFactory.h"

#include "VoreealBasicVolume.h"

#include "Formats/MagicaVoxelFileFormat.h"
#include "Formats/QubicleBinaryFileFormat.h"

#include <AssetToolsModule.h>
#include <AssetRegistryModule.h>
#include <PackageTools.h>
#include <StringConv.h>

#define LOCTEXT_NAMESPACE "VoreealEditor"

UVoreealVolumeImporterFactory::UVoreealVolumeImporterFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
	SupportedClass = UBasicVolume::StaticClass();

	bEditorImport = true;
	bText = true;

	FileFormats.Add(new FMagicaVoxelFileFormat());
	FileFormats.Add(new FQubicleBinaryFileFormat());

	for (auto fileFormat : FileFormats)
	{
		FString text = FString::Printf(TEXT("%s;%s"), *fileFormat->GetExtension(), *fileFormat->GetFileDescription());
		Formats.Add(text);
	}
}

UVoreealVolumeImporterFactory::~UVoreealVolumeImporterFactory()
{
	for (auto fileFormat : FileFormats)
	{
		delete fileFormat;
	}
}

FText UVoreealVolumeImporterFactory::GetToolTip() const
{
	return LOCTEXT("UVoreealVolumeImporterFactoryDescription", "Voreeal Volumes");
}

bool UVoreealVolumeImporterFactory::FactoryCanImport(const FString& Filename)
{
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *Filename))
	{
		return false;
	}

	FString FilenameExtension = FPaths::GetExtension(Filename);
	FString ErrorMessage;

	for (auto fileFormat : FileFormats)
	{
		if (fileFormat->GetExtension() == FilenameExtension)
		{
			FMemoryReader Ar = FMemoryReader(TheBinaryArray, true);
			if (fileFormat->IsFile(Ar, ErrorMessage))
			{
				return true;
			}
		}
	}

	return false;
}

UObject* UVoreealVolumeImporterFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, 
	const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	Flags |= RF_Transactional;

	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	UBasicVolume* Result = NewObject<UBasicVolume>(InParent, InName, Flags);

	FString FilenameExtension = FPaths::GetExtension(CurrentFilename);

	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *CurrentFilename))
	{
		UE_LOG(LogVoreealVolumeImporter, Error, TEXT("Invalid File"));
		return nullptr;
	}

	FString ErrorMessage;
	bool Success = false;

	for (auto fileFormat : FileFormats)
	{
		if (fileFormat->GetExtension() == FilenameExtension)
		{
			FMemoryReader IsAr = FMemoryReader(TheBinaryArray, true);
			if (fileFormat->IsFile(IsAr, ErrorMessage))
			{
				// TODO: Reset memory reader position instead
				FMemoryReader Ar = FMemoryReader(TheBinaryArray, true);
				if (fileFormat->ReadFile(Ar, Result, ErrorMessage))
				{
					Success = true;
				}
				else
				{
					FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
					return nullptr;
				}
			}
		}
	}

	if (!Success)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Found no suitable importer!")));
		return nullptr;
	}

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

bool UVoreealVolumeImporterFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
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

void UVoreealVolumeImporterFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (UBasicVolume* volume = Cast<UBasicVolume>(Obj))
	{
		if (ensure(NewReimportPaths.Num() == 1))
		{
			volume->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
		}
	}
}

EReimportResult::Type UVoreealVolumeImporterFactory::Reimport(UObject* Obj)
{
	if (UBasicVolume* Volume = Cast<UBasicVolume>(Obj))
	{
		FString Filename = Volume->AssetImportData->GetFirstFilename();
		FString FilenameExtension = FPaths::GetExtension(Filename);

		TArray<uint8> TheBinaryArray;
		if (!FFileHelper::LoadFileToArray(TheBinaryArray, *CurrentFilename))
		{
			UE_LOG(LogVoreealVolumeImporter, Error, TEXT("Invalid File"));
			return EReimportResult::Failed;
		}

		FString ErrorMessage;
		bool Success = false;

		for (auto fileFormat : FileFormats)
		{
			if (fileFormat->GetExtension() == FilenameExtension)
			{
				FMemoryReader IsAr = FMemoryReader(TheBinaryArray, true);
				if (fileFormat->IsFile(IsAr, ErrorMessage))
				{
					// TODO: Reset memory reader position instead
					FMemoryReader Ar = FMemoryReader(TheBinaryArray, true);
					if (fileFormat->ReadFile(Ar, Volume, ErrorMessage))
					{
						Success = true;
					}
					else
					{
						FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(ErrorMessage));
						return EReimportResult::Failed;
					}
				}
			}
		}

		if (!Success)
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Found no suitable importer!")));
			return EReimportResult::Failed;
		}

		FMemoryWriter MemoryWriter{ Volume->ImportedData, true };
		FArchive Ar{ MemoryWriter };
		// TODO: !!!
		//Voreeal::Export(polyVoxVolume, Ar);

		Volume->PostEditChange();

		return EReimportResult::Succeeded;
	}
	return EReimportResult::Failed;
}

int32 UVoreealVolumeImporterFactory::GetPriority() const
{
	return ImportPriority;
}

#undef LOCTEXT_NAMESPACE
