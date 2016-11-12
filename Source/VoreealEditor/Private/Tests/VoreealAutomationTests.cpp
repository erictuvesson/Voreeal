#include "VoreealEditorPrivatePCH.h"
#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "AssetEditorManager.h"

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoreealTest, "Voreeal Creation and Duplication", EAutomationTestFlags::EditorContext);
//
//// https://docs.unrealengine.com/latest/INT/Programming/Automation/TechnicalGuide/index.html
//// https://github.com/EpicGames/UnrealEngine/blob/4.5/Engine/Source/Editor/UnrealEd/Private/Tests/EditorAssetAutomationTests.cpp#L431
//bool FVoreealTest::RunTest(const FString& Parameters)
//{
//	// FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
//	
//	//const FString FullPath = CreateAssetHelper::GetFullPath();
//	//const FString GamePath = CreateAssetHelper::GetGamePath();
//
//	////Create the folder if it doesn't already exist
//	//if (!IFileManager::Get().DirectoryExists(*FullPath))
//	//{
//	//	//Make the new folder
//	//	IFileManager::Get().MakeDirectory(*FullPath, true);
//
//	//	// Add the path to the asset registry
//	//	AssetRegistryModule.Get().AddPath(GamePath);
//
//	//	// Notify 'asset path changed' delegate
//	//	FContentBrowserModule::FOnAssetPathChanged& PathChangedDelegate = ContentBrowserModule.GetOnAssetPathChanged();
//	//	if (PathChangedDelegate.IsBound())
//	//	{
//	//		PathChangedDelegate.Broadcast(GamePath);
//	//	}
//	//}
//
//	//ASSET_TEST_CREATE(UTextureRenderTargetCube, UTextureRenderTargetCubeFactoryNew, CRT, )
//
//	return true;
//}
