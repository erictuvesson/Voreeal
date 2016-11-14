#include "VoreealPrivatePCH.h"
#include "VoreealBlueprintLibrary.h"

#include "DrawDebugHelpers.h"

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
