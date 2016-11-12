#include "VoreealPrivatePCH.h"
#include "VoreealBlueprintLibrary.h"

#include "DrawDebugHelpers.h"

void UVoreealBlueprintLibrary::DrawDebugRegion(UObject* WorldContextObject, const FTransform& Transform, 
	const FVoreealRegion& Region, const FColor& Color, float Duration, float Thickness)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		FVector Extent = Region.Size() * Transform.GetScale3D();
		FVector Center = Transform.GetLocation();
		DrawDebugBox(World, Center, Extent, Color);
	}
}
