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
		// TODO: set default size from settings
		Resize(FIntVector(64, 64, 64));
	}
}

#if WITH_EDITORONLY_DATA
void UBasicVolume::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	Super::GetAssetRegistryTags(OutTags);
	
	//if (AssetImportData)
	//{
	//	OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
	//}
}

void UBasicVolume::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

bool UBasicVolume::IsValid() const
{
	return Volume != nullptr;
}

FVoreealMesh UBasicVolume::ExtractMesh(const FVoreealExtractorOptions& Options)
{
	return ExtractMeshHelper(Volume.get(), ExtractorType, Options);
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

void UBasicVolume::Internal_SetSize(const FVoreealRegion& Region, bool New)
{
	if (New)
	{
		Volume.reset(nullptr);
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

		PolyVox::VolumeResampler<VolumeType, VolumeType> volumeResampler(Volume.get(), Volume->getEnclosingRegion(), newVolume, newVolume->getEnclosingRegion());
		volumeResampler.execute();

		Volume.reset(newVolume);
	}
	else
	{
		Volume.reset(new VolumeType((PolyVox::Region)NewRegion));
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
