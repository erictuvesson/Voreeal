#include "VoreealPrivatePCH.h"
#include "VoreealBasicVolume.h"

#include <PolyVox/VolumeResampler.h>

#if WITH_EDITORONLY_DATA
#include <ThumbnailRendering/SceneThumbnailInfo.h>
#endif

UBasicVolume::UBasicVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Resize(FIntVector(64, 64, 64));
}

void UBasicVolume::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// If data is empty serialize the volume
	// This can happen because the volume was created at runtime
	if (!Ar.IsLoading() && ImportedData.Num() == 0)
	{
		FVoreealRegion Region = GetEnclosingRegion();
		FBufferArchive Arr;
		SerializeVolume(Arr, Region);
		ImportedData = Arr;
	}

	Ar << ImportedData;

#if WITH_EDITORONLY_DATA
	if (Ar.IsLoading() && (AssetImportData == nullptr))
	{
		// AssetImportData should always be valid
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
}

#if WITH_EDITOR
void UBasicVolume::PreEditChange(UProperty* PropertyAboutToChange)
{
	Super::PreEditChange(PropertyAboutToChange);
}

void UBasicVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UBasicVolume::CanEditChange(const UProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}
#endif

#if WITH_EDITORONLY_DATA
void UBasicVolume::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	Super::GetAssetRegistryTags(OutTags);
	
	if (AssetImportData)
	{
		OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
	}
}
#endif

bool UBasicVolume::IsValid() const
{
	return Volume.IsValid();
}

void UBasicVolume::GetVoxel(const FVector& Location, uint8& Material, uint8& Density)
{
	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
	PolyVox::MaterialDensityPair88 Data = Volume->getVoxel(pos);
	Material = Data.getMaterial();
	Density = Data.getDensity();
}

FVoreealMesh UBasicVolume::ExtractMesh(const FVoreealExtractorOptions& Options)
{
	return ExtractMeshHelper(Volume.Get(), ExtractorType, Options);
}

bool UBasicVolume::Internal_SetVoxel(const FVector& Location, const uint8& Material, const uint8& Density)
{
	PolyVox::Vector3DInt32 pos(Location.X, Location.Y, Location.Z);
	Volume->setVoxel(pos, PolyVox::MaterialDensityPair88(Material, Density));
	return true;
}

void UBasicVolume::Internal_SetSize(const FVoreealRegion& Region, bool New)
{
	if (New)
	{
		Volume.Reset();
		Volume = nullptr;
	}
	ResizeRegion(Region);
}

FVoreealRegion UBasicVolume::GetEnclosingRegion() const
{
	return FVoreealRegion(Volume->getEnclosingRegion());
}

void UBasicVolume::Resize(const FIntVector& NewSize)
{
	ResizeRegion(FVoreealRegion(0, 0, 0, NewSize.X, NewSize.Y, NewSize.Z));
}

void UBasicVolume::ResizeRegion(const FVoreealRegion& NewRegion)
{
	if (IsValid())
	{
		VolumeType* newVolume = new VolumeType((PolyVox::Region)NewRegion);

		//PolyVox::VolumeResampler<VolumeType, VolumeType> volumeResampler(Volume.Get(), Volume->getEnclosingRegion(), newVolume, newVolume->getEnclosingRegion());
		//volumeResampler.execute();

		Volume.Reset();
		Volume = MakeShareable(newVolume);
	}
	else
	{
		Volume.Reset();
		Volume = MakeShareable(new VolumeType((PolyVox::Region)NewRegion));
	}
}

#if WITH_EDITOR
void UBasicVolume::ValidateSocketNames()
{
	TSet<FName> SocketNames;
	struct Local
	{
		static FName GetUniqueName(const TSet<FName>& InSocketNames, FName Name)
		{
			int Counter = Name.GetNumber();
			FName TestName;
			do
			{
				TestName = Name;
				TestName.SetNumber(++Counter);
			} while (InSocketNames.Contains(TestName));

			return TestName;
		}
	};

	bool bHasChanged = false;
	for (int32 SocketIndex = 0; SocketIndex < Sockets.Num(); ++SocketIndex)
	{
		FBasicVolumeSocket& Socket = Sockets[SocketIndex];
		if (Socket.SocketName.IsNone())
		{
			Socket.SocketName = Local::GetUniqueName(SocketNames, FName(TEXT("Socket")));
			bHasChanged = true;
		}
		else if (SocketNames.Contains(Socket.SocketName))
		{
			Socket.SocketName = Local::GetUniqueName(SocketNames, Socket.SocketName);
			bHasChanged = true;
		}

		// Add the corrected name
		SocketNames.Add(Socket.SocketName);
	}

	if (bHasChanged)
	{
		PostEditChange();
	}
}

void UBasicVolume::RemoveSocket(FName SocketName)
{
	Sockets.RemoveAll([=](const FBasicVolumeSocket& Socket)
	{ 
		return Socket.SocketName == SocketName; 
	});
}
#endif

FBasicVolumeSocket* UBasicVolume::FindSocket(FName SocketName)
{
	for (int32 SocketIndex = 0; SocketIndex < Sockets.Num(); ++SocketIndex)
	{
		FBasicVolumeSocket& Socket = Sockets[SocketIndex];
		if (Socket.SocketName == SocketName)
		{
			return &Socket;
		}
	}

	return nullptr;
}

void UBasicVolume::QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const
{
	for (int32 SocketIndex = 0; SocketIndex < Sockets.Num(); ++SocketIndex)
	{
		const FBasicVolumeSocket& Socket = Sockets[SocketIndex];
		new (OutSockets) FComponentSocketDescription(Socket.SocketName, EComponentSocketType::Socket);
	}
}
