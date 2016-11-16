#include "VoreealPrivatePCH.h"
#include "VoreealVolumeComponent.h"
#include "VoreealVolume.h"

UVoreealVolumeComponent::UVoreealVolumeComponent(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Voreeal/VertexColorMaterial.VertexColorMaterial'"));

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	MeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	if (Material.Object != NULL)
	{
		MeshComponent->SetMaterial(0, Material.Object);
	}

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
		UE_LOG(LogVoreeal, Warning, TEXT("Task: Volume is null.  (This should never happen!)"));
		return;
	}

	if (Options.Region.Width <= 0 ||
		Options.Region.Height <= 0)
	{
		UE_LOG(LogVoreeal, Warning, TEXT("Task: Region is to small. (This should never happen!)"));
		return;
	}

	if (Runnable && Identifier != -1)
	{
		Runnable->AddTask(Identifier, [Volume, Options]() -> FVoreealMesh
		{
			// TODO: Ensure that all tasks are done before deleting volume
			return Volume->ExtractMesh(Options);
		});
	}
	else
	{
		UE_LOG(LogVoreeal, Warning, TEXT("Task: Runnable? (This should never happen!)"));
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
