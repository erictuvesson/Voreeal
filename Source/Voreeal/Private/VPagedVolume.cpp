#include "VoreealPrivatePCH.h"
#include "VPagedVolume.h"

//UPagedVolume::UPagedVolume(const class FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}
//
//void UPagedVolume::Serialize(FArchive& Ar)
//{
//	Super::Serialize(Ar);
////
////	Ar << ImportedData;
////
////#if WITH_EDITORONLY_DATA
////	if (Ar.IsLoading() && (AssetImportData == nullptr))
////	{
////		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
////	}
////#endif
//}
//
//void UPagedVolume::PostInitProperties()
//{
////#if WITH_EDITORONLY_DATA
////	if (!HasAnyFlags(RF_ClassDefaultObject))
////	{
////		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
////	}
////#endif
//	Super::PostInitProperties();
//}
//
//void UPagedVolume::PostLoad()
//{
//	Super::PostLoad();
//
//	//if (ImportedData.Num() > 0)
//	//{
//	//	FMemoryReader Ar{ ImportedData, true };
//
//	//	FRegion region;
//	//	UBasicVolume* volume;
//
//	//	DeserializeVolume(Ar, volume, region);
//
//	//	Volume.reset(volume);
//	//	OnChanged.Broadcast();
//	//}
//}
//
//void UPagedVolume::ExtractMesh(const FRegion& Region, const int32& LOD, FProceduralMesh& Mesh)
//{
//	Internal_ExtractMesh(Volume.get(), ExtractorType, Region, LOD, Mesh);
//}
//
//bool UPagedVolume::IsValid() const
//{
//    return Volume != nullptr;
//}
//
//bool UPagedVolume::Internal_SetVoxel(const FVector& Location, const uint32& Data)
//{
//    PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
//    Volume->setVoxel(pos, Data);
//    return true;
//}
//
//void UPagedVolume::Internal_GetVoxel(const FVector& Location, uint32& Data)
//{
//	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
//	Data = Volume->getVoxel(pos);
//}
//
//bool UPagedVolume::AddChunk(int32 X, int32 Y, int32 Z)
//{
//    if (IsValid())
//    {
//        // TODO: 
//        return true;
//    }
//    return false;
//}
//
//bool UPagedVolume::RemoveChunk(int32 X, int32 Y, int32 Z)
//{
//    if (IsValid())
//    {
//        // TODO: 
//        return true;
//    }
//    return false;
//}
//
//void UPagedVolume::RemoveAllChunks()
//{
//    if (IsValid())
//    {
//		// TODO:
//        //Volume->flush();
//    }
//}
//
//uint32 UPagedVolume::CalculateSizeInBytes()
//{
//    return IsValid() ? Volume->calculateSizeInBytes() : 0;
//}
