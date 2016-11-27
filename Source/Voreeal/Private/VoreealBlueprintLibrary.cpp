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
	return 0;
}

void UVoreealBlueprintLibrary::DrawDebugTaskText(UObject* WorldContextObject, int32 Key, const FColor& Color)
{
	int32 TasksCount = GetRunningTaskCount();

	GEngine->AddOnScreenDebugMessage(Key, 5.0, Color, FString::Printf(TEXT("Tasks Running: %d"), TasksCount));
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
