#include "VoreealPrivatePCH.h"
#include "VBasicVolume.h"

#include <PolyVox/VolumeResampler.h>

#if WITH_EDITORONLY_DATA
#include <ThumbnailRendering/SceneThumbnailInfo.h>
#endif

UBasicVolume::UBasicVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UBasicVolume::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar << ImportedData;

#if WITH_EDITORONLY_DATA
	if (Ar.IsLoading() && (AssetImportData == nullptr))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
}

void UBasicVolume::PostInitProperties()
{
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
	Super::PostInitProperties();
}

void UBasicVolume::PostLoad()
{
	Super::PostLoad();

	if (ImportedData.Num() > 0)
	{
		FMemoryReader Ar{ ImportedData, true };
		DeserializeVolume(Ar);
		OnChanged.Broadcast(GetEnclosingRegion());
	}
	else
	{
		// TODO: Move?
		Resize(FRegion(0, 0, 0, 64, 64, 64));
	}
}

#if WITH_EDITORONLY_DATA
void UBasicVolume::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	if (AssetImportData)
	{
		OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
	}

	Super::GetAssetRegistryTags(OutTags);
}
#endif

void UBasicVolume::ExtractMesh(const FRegion& Region, const int32& LOD, FProceduralMesh& Mesh)
{
	Internal_ExtractMesh(Volume.get(), ExtractorType, Region, LOD, Mesh);
}

bool UBasicVolume::IsValid() const
{
    return Volume != nullptr;
}

bool UBasicVolume::Internal_SetVoxel(const FVector& Location, const uint32& Data)
{
	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
	Volume->setVoxel(pos, Data);
    return true;
}

void UBasicVolume::Internal_GetVoxel(const FVector& Location, uint32& Data)
{
	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
	Data = Volume->getVoxel(pos);
}

void UBasicVolume::Internal_SetSize(const FRegion& Region, bool New)
{
	if (New)
	{
		Volume.reset(nullptr);
	}

	Resize(Region);
}

FRegion UBasicVolume::GetEnclosingRegion() const
{
	return FRegion(Volume->getEnclosingRegion());
}

void UBasicVolume::Resize(const FRegion& NewRegion)
{
	if (IsValid())
	{
		VolumeType* newVolume = new VolumeType((PolyVox::Region)NewRegion);

		PolyVox::VolumeResampler<VolumeType, VolumeType> volumeResampler(Volume.get(), Volume->getEnclosingRegion(), newVolume, newVolume->getEnclosingRegion());
		volumeResampler.execute();

		Volume.reset(newVolume);
	}
	else
	{
		Volume.reset(new VolumeType((PolyVox::Region)NewRegion));
	}
}
