#include "VoreealEditorPrivatePCH.h"
#include "VVolumeThumbnailRenderer.h"

#include <EngineModule.h>
#include <RendererInterface.h>

// https://github.com/EpicGames/UnrealEngine/blob/311e18ff369078e192a83f27834b45bdb288168a/Engine/Source/Editor/UnrealEd/Private/ThumbnailHelpers.cpp#L377
// https://github.com/EpicGames/UnrealEngine/blob/e607ba4d978c08a26e8e8e629dec0884bb161770/Engine/Source/Editor/UnrealEd/Classes/ThumbnailRendering/SkeletalMeshThumbnailRenderer.h
// https://github.com/EpicGames/UnrealEngine/blob/e607ba4d978c08a26e8e8e629dec0884bb161770/Engine/Source/Editor/UnrealEd/Private/ThumbnailRendering/SkeletalMeshThumbnailRenderer.cpp

FVVolumeThumbnailScene::FVVolumeThumbnailScene()
	: FThumbnailPreviewScene()
{
	bForceAllUsedMipsResident = false;
	// Create preview actor
	// checked
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.bNoFail = true;
	SpawnInfo.ObjectFlags = RF_Transient;
	PreviewActor = GetWorld()->SpawnActor<ABasicVolumeActor>(SpawnInfo);

	PreviewActor->SetActorEnableCollision(false);
}

void FVVolumeThumbnailScene::SetVolume(UBasicVolume* Volume)
{
	PreviewActor->GetBasicVolumeComponent()->SetBasicVolume(Volume);

	if (Volume)
	{
		// TODO: Set actor location

		FTransform MeshTransform = FTransform::Identity;

		PreviewActor->SetActorLocation(FVector(0, 0, 0), false);
		//PreviewActor->MeshComponent->UpdateBounds();

		// Center the mesh at the world origin then offset to put it on top of the plane
		//const float BoundsZOffset = GetBoundsZOffset(PreviewActor->MeshComponent->Bounds);
		//PreviewActor->SetActorLocation(-PreviewActor->MeshComponent->Bounds.Origin + FVector(0, 0, BoundsZOffset), false);
	}
}

void FVVolumeThumbnailScene::GetViewMatrixParameters(const float InFOVDegrees, FVector& OutOrigin, float& OutOrbitPitch, float& OutOrbitYaw, float& OutOrbitZoom) const
{
	// TODO: Access ThumbnailInfo and Bounds

	//check(PreviewActor->GetBasicVolumeComponent());
	//
	//const float HalfFOVRadians = FMath::DegreesToRadians<float>(InFOVDegrees) * 0.5f;
	//const float HalfMeshSize = PreviewActor->MeshComponent->Bounds.SphereRadius;
	//const float BoundsZOffset = GetBoundsZOffset(PreviewActor->MeshComponent->Bounds);
	//const float TargetDistance = HalfMeshSize / FMath::Tan(HalfFOVRadians);
	//
	////@TODO: Switch to USceneThumbnailInfoWithPrimitive
	////@TODO: How does Mesh etc show more infomation about the asset?
	//USceneThumbnailInfo* ThumbnailInfo = Cast<USceneThumbnailInfo>(PreviewActor->GetBasicVolumeComponent()->ThumbnailInfo);
	//if (ThumbnailInfo)
	//{
	//	if (TargetDistance + ThumbnailInfo->OrbitZoom < 0)
	//	{
	//		ThumbnailInfo->OrbitZoom = -TargetDistance;
	//	}
	//}
	//else
	//{
	//	ThumbnailInfo = USceneThumbnailInfo::StaticClass()->GetDefaultObject<USceneThumbnailInfo>();
	//}
	//
	//OutOrigin = FVector(0, 0, -BoundsZOffset);
	//OutOrbitPitch = ThumbnailInfo->OrbitPitch;
	//OutOrbitYaw = ThumbnailInfo->OrbitYaw;
	//OutOrbitZoom = TargetDistance + ThumbnailInfo->OrbitZoom;
}



UVVolumeThumbnailRenderer::UVVolumeThumbnailRenderer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ThumbnailScene = nullptr;
}

bool UVVolumeThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	UBasicVolume* VoreealVolume = Cast<UBasicVolume>(Object);
	return VoreealVolume != nullptr;
}

void UVVolumeThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas)
{
	UBasicVolume* VoreealVolume = Cast<UBasicVolume>(Object);
	if (VoreealVolume != nullptr)
	{
		if (ThumbnailScene == nullptr)
		{
			ThumbnailScene = new FVVolumeThumbnailScene();
		}

		ThumbnailScene->SetVolume(VoreealVolume);
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(RenderTarget, ThumbnailScene->GetScene(), FEngineShowFlags(ESFIM_Game))
			.SetWorldTimes(FApp::GetCurrentTime() - GStartTime, FApp::GetDeltaTime(), FApp::GetCurrentTime() - GStartTime));

		ViewFamily.EngineShowFlags.DisableAdvancedFeatures();
		ViewFamily.EngineShowFlags.MotionBlur = 0;
		ViewFamily.EngineShowFlags.LOD = 0;

		ThumbnailScene->GetView(&ViewFamily, X, Y, Width, Height);
		GetRendererModule().BeginRenderingViewFamily(Canvas, &ViewFamily);
		ThumbnailScene->SetVolume(nullptr);
	}
}

void UVVolumeThumbnailRenderer::BeginDestroy()
{
	if (ThumbnailScene != nullptr)
	{
		delete ThumbnailScene;
		ThumbnailScene = nullptr;
	}

	Super::BeginDestroy();
}
