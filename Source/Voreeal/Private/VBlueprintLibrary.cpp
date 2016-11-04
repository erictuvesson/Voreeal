#include "VoreealPrivatePCH.h"
#include "VBlueprintLibrary.h"

#include "DrawDebugHelpers.h"

void UVBlueprintLibrary::GetCenter(const FRegion& Region, FVector& Result)
{
	Result = FVector(Region.X + (Region.Width / 2), Region.Y + (Region.Height / 2), Region.Z + (Region.Depth / 2));
}

void UVBlueprintLibrary::GetUpperCorner(const FRegion& Region, FVector& Result)
{
	Result = FVector(Region.X, Region.Y, Region.Z);
}

void UVBlueprintLibrary::GetLowerCorner(const FRegion& Region, FVector& Result)
{
	Result = FVector(Region.X + Region.Width, Region.Y + Region.Height, Region.Z + Region.Depth);
}

void UVBlueprintLibrary::Grow(const FRegion& Region, int32 Width, int32 Height, int32 Depth, FRegion& Result)
{
	Result = Region;
	Result.X -= Width;
	Result.Y -= Height;
	Result.Z -= Depth;
	Result.Width += Width * 2;
	Result.Height += Height * 2;
	Result.Depth += Depth * 2;
}

void UVBlueprintLibrary::ShiftUpperCorner(const FRegion& Region, int32 X, int32 Y, int32 Z, FRegion& Result)
{
	Result = Region;
	Result.X += X;
	Result.Y += Y;
	Result.Z += Z;
	Result.Width -= X;
	Result.Height -= Y;
	Result.Depth -= Z;
}

void UVBlueprintLibrary::ShiftLowerCorner(const FRegion& Region, int32 X, int32 Y, int32 Z, FRegion& Result)
{
	Result = Region;
	Result.Width += X;
	Result.Height += Y;
	Result.Depth += Z;
}

bool UVBlueprintLibrary::Contains(const FRegion& Region1, const FRegion& Region2)
{
	return (Region2.X < (Region1.X + Region1.Width))
		&& (Region1.X < (Region2.X + Region2.Width))
		&& (Region2.Y < (Region1.Y + Region1.Height))
		&& (Region1.Y < (Region2.Y + Region2.Height))
		&& (Region2.Z < (Region1.Z + Region1.Depth))
		&& (Region1.Z < (Region2.Z + Region2.Depth));
}

bool UVBlueprintLibrary::Intersect(const FRegion& Region1, const FRegion& Region2)
{
	return (Region2.X < (Region1.X + Region1.Width))
		&& (Region1.X < (Region2.X + Region2.Width))
		&& (Region2.Y < (Region1.Y + Region1.Height))
		&& (Region1.Y < (Region2.Y + Region2.Height))
		&& (Region2.Z < (Region1.Z + Region1.Depth))
		&& (Region1.Z < (Region2.Z + Region2.Depth));
}

void UVBlueprintLibrary::DrawDebugRegion(UObject* WorldContextObject, const FTransform& Transform, 
	const FRegion& Region, const FColor& Color, float Duration, float Thickness)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (World)
	{
		FVector Extent = Region.Size() * Transform.GetScale3D();
		FVector Center = Transform.GetLocation();
		DrawDebugBox(World, Center, Extent, Color);
	}
}
