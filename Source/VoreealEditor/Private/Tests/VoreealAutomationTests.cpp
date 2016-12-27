#include "VoreealEditorPrivatePCH.h"

#include "VoreealBasicVolumeComponent.h"

#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "AssetEditorManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoreealTest, "Voreeal.General testing of the library", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoreealContentTest, "Voreeal.Content.Asset Creation and Duplication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

bool FVoreealTest::RunTest(const FString& Parameters)
{
	AddLogItem(TEXT("Creating Basic Volume"));
	UBasicVolume* Volume = NewObject<UBasicVolume>();

	AddLogItem(TEXT("Resizing Basic Volume to 256x256x256"));
	Volume->Resize(FIntVector(256, 256, 256));

	AddLogItem(TEXT("Creating Sphere in Basic Volume"));

	const int32 SphereRadius = 128;

	FVector SphereCenter = Volume->GetEnclosingRegion().Size() / 2;
	for (int32 x = 0; x < 256; x++)
	{
		for (int32 y = 0; y < 256; y++)
		{
			for (int32 z = 0; z < 256; z++)
			{
				FVector CurrentPosition(x, y, z);
				if (FVector::Dist(CurrentPosition, SphereCenter) <= SphereRadius)
				{
					Volume->SetVoxel(CurrentPosition, FColor::Red, 255);
				}
			}
		}
	}

	AddLogItem(TEXT("Successfully handled Basic Volume"));

	//AddLogItem(TEXT("Creating Basic Volume Component"));
	//UBasicVolumeComponent* VolumeComponent = NewObject<UBasicVolumeComponent>();
	//VolumeComponent->SetBasicVolume(Volume);
	//
	//AddLogItem(TEXT("Successfully created Basic Volume Component"));
	//
	//AddLogItem(TEXT("Work with Basic Volume Component"));
	//VolumeComponent->Update();
	//
	//// TODO: 

	Volume->ConditionalBeginDestroy();
	Volume = nullptr;

	return true;
}

bool FVoreealContentTest::RunTest(const FString& Parameters)
{
	// https://github.com/EpicGames/UnrealEngine/blob/4.5/Engine/Source/Editor/UnrealEd/Private/Tests/EditorAssetAutomationTests.cpp#L431

	// FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	
	//const FString FullPath = CreateAssetHelper::GetFullPath();
	//const FString GamePath = CreateAssetHelper::GetGamePath();

	////Create the folder if it doesn't already exist
	//if (!IFileManager::Get().DirectoryExists(*FullPath))
	//{
	//	//Make the new folder
	//	IFileManager::Get().MakeDirectory(*FullPath, true);

	//	// Add the path to the asset registry
	//	AssetRegistryModule.Get().AddPath(GamePath);

	//	// Notify 'asset path changed' delegate
	//	FContentBrowserModule::FOnAssetPathChanged& PathChangedDelegate = ContentBrowserModule.GetOnAssetPathChanged();
	//	if (PathChangedDelegate.IsBound())
	//	{
	//		PathChangedDelegate.Broadcast(GamePath);
	//	}
	//}

	//ASSET_TEST_CREATE(UTextureRenderTargetCube, UTextureRenderTargetCubeFactoryNew, CRT, )

	return true;
}
