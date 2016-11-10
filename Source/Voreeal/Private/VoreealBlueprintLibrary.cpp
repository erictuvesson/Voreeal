#include "VoreealPrivatePCH.h"
#include "VoreealBlueprintLibrary.h"

#include "DrawDebugHelpers.h"

void UVoreealBlueprintLibrary::GetCenter(const FVoreealRegion& Region, FVector& Result)
{
	Result = FVector(Region.X + (Region.Width / 2), Region.Y + (Region.Height / 2), Region.Z + (Region.Depth / 2));
}

void UVoreealBlueprintLibrary::GetUpperCorner(const FVoreealRegion& Region, FVector& Result)
{
	Result = FVector(Region.X, Region.Y, Region.Z);
}

void UVoreealBlueprintLibrary::GetLowerCorner(const FVoreealRegion& Region, FVector& Result)
{
	Result = FVector(Region.X + Region.Width, Region.Y + Region.Height, Region.Z + Region.Depth);
}

void UVoreealBlueprintLibrary::Grow(const FVoreealRegion& Region, int32 Width, int32 Height, int32 Depth, FVoreealRegion& Result)
{
	Result = Region;
	Result.X -= Width;
	Result.Y -= Height;
	Result.Z -= Depth;
	Result.Width += Width * 2;
	Result.Height += Height * 2;
	Result.Depth += Depth * 2;
}

void UVoreealBlueprintLibrary::ShiftUpperCorner(const FVoreealRegion& Region, int32 X, int32 Y, int32 Z, FVoreealRegion& Result)
{
	Result = Region;
	Result.X += X;
	Result.Y += Y;
	Result.Z += Z;
	Result.Width -= X;
	Result.Height -= Y;
	Result.Depth -= Z;
}

void UVoreealBlueprintLibrary::ShiftLowerCorner(const FVoreealRegion& Region, int32 X, int32 Y, int32 Z, FVoreealRegion& Result)
{
	Result = Region;
	Result.Width += X;
	Result.Height += Y;
	Result.Depth += Z;
}

bool UVoreealBlueprintLibrary::Contains(const FVoreealRegion& Region1, const FVoreealRegion& Region2)
{
	return (Region2.X < (Region1.X + Region1.Width))
		&& (Region1.X < (Region2.X + Region2.Width))
		&& (Region2.Y < (Region1.Y + Region1.Height))
		&& (Region1.Y < (Region2.Y + Region2.Height))
		&& (Region2.Z < (Region1.Z + Region1.Depth))
		&& (Region1.Z < (Region2.Z + Region2.Depth));
}

bool UVoreealBlueprintLibrary::Intersect(const FVoreealRegion& Region1, const FVoreealRegion& Region2)
{
	return (Region2.X < (Region1.X + Region1.Width))
		&& (Region1.X < (Region2.X + Region2.Width))
		&& (Region2.Y < (Region1.Y + Region1.Height))
		&& (Region1.Y < (Region2.Y + Region2.Height))
		&& (Region2.Z < (Region1.Z + Region1.Depth))
		&& (Region1.Z < (Region2.Z + Region2.Depth));
}

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
