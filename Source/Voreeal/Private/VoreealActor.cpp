#include "VoreealPrivatePCH.h"
#include "VoreealActor.h"

AVoreealActor::AVoreealActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AVoreealActor::WorldPositionToVolumePosition(const FVector& worldPosition, FVector& result) const
{
	result = ActorToWorld().InverseTransformPosition(worldPosition);
}

void AVoreealActor::VolumePositionToWorldPosition(const FVector& localPosition, FVector& result) const
{
	result = ActorToWorld().TransformPosition(localPosition);
}

void AVoreealActor::WorldDirectionToVolumeDirection(const FVector& worldDirection, FVector& result) const
{
	result = ActorToWorld().InverseTransformVectorNoScale(worldDirection);
}

void AVoreealActor::VolumeDirectionToWorldDirection(const FVector& localDirection, FVector& result) const
{
	result = ActorToWorld().TransformVectorNoScale(localDirection);
}

bool AVoreealActor::PickFirstSolidVoxel(const FVector& LocalStartPosition, const FVector& LocalDirection, FVector& VoxelLocation) const
{
	// TODO: Raycast
	return false;
}

bool AVoreealActor::PickLastSolidVoxel(const FVector& LocalStartPosition, const FVector& LocalDirection, FVector& VoxelLocation) const
{
	// TODO: Raycast
	return false;
}