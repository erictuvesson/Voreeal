#include "VoreealPrivatePCH.h"
#include "VoreealVolumeComponent.h"
#include "VoreealVolume.h"

UVoreealVolumeComponent::UVoreealVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitWorker();
	Identifier = Runnable ? Runnable->RequestNewIdentifier() : -1;
}

bool UVoreealVolumeComponent::FindFinishedTask(TSharedPtr<FVoreealMesh>& Result)
{
	Result = nullptr;

	if (Runnable && Identifier != -1)
	{
		auto R = Runnable->GetTask(Identifier);
		if (R.IsValid())
		{
			Result = R;
			return true;
		}
	}

	return false;
}

void UVoreealVolumeComponent::AddTask(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options)
{
	if (Volume == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task: Volume is null."));
		return; // Volume is null
	}

	if (!Volume->MeshExtractor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task: Mesh Extractor is null."));
		return; // Mesh Extractor is null
	}

	if (Options.Region.Width <= 0 ||
		Options.Region.Height <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task: Region is to small."));
		return; // Region is to small
	}

	if (Runnable && Identifier != -1)
	{
		Runnable->AddTask(Identifier, [Volume, Options]() -> FVoreealMesh
		{
			return Volume->MeshExtractor.GetDefaultObject()
				->ExtractMesh(Volume, Options);
		});
	}
	else
	{
		// TODO: Log error
	}
}

FVoreealWorker* UVoreealVolumeComponent::Runnable = nullptr;
void UVoreealVolumeComponent::InitWorker()
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FVoreealWorker();
	}
}
