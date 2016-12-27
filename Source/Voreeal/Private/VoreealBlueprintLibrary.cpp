#include "VoreealPrivatePCH.h"
#include "VoreealBlueprintLibrary.h"

#include "VoreealBasicVolumeComponent.h"

#include "DrawDebugHelpers.h"

int32 UVoreealBlueprintLibrary::GetRunningTaskCount()
{
	if (UBasicVolumeComponent::Runnable != nullptr)
	{
		return UBasicVolumeComponent::Runnable->GetRunningTaskCount();
	}
	return -1;
}

void UVoreealBlueprintLibrary::DrawDebugRegion(UObject* WorldContextObject, const FTransform& Transform,
	const FVoreealRegion& Region, const FLinearColor& LinearColor, float Duration, float Thickness)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		FVector Extent = Region.Size() * Transform.GetScale3D();
		FVector Offset = Region.GetLower() * Transform.GetScale3D();
		FVector Center = Transform.GetLocation() + Extent + Offset;
		FColor Color = LinearColor.ToFColor(false);
		DrawDebugBox(World, Center, Extent, Transform.Rotator().Quaternion(), Color);
	}
}
