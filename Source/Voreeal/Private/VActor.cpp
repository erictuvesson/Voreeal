#include "VoreealPrivatePCH.h"
#include "VActor.h"

AVActor::AVActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AVActor::WorldPositionToVolumePosition(const FVector& worldPosition, FVector& result) const
{
	result = ActorToWorld().InverseTransformPosition(worldPosition);
}

void AVActor::VolumePositionToWorldPosition(const FVector& localPosition, FVector& result) const
{
	result = ActorToWorld().TransformPosition(localPosition);
}

void AVActor::WorldDirectionToVolumeDirection(const FVector& worldDirection, FVector& result) const
{
	result = ActorToWorld().InverseTransformVectorNoScale(worldDirection);
}

void AVActor::VolumeDirectionToWorldDirection(const FVector& localDirection, FVector& result) const
{
	result = ActorToWorld().TransformVectorNoScale(localDirection);
}




bool AVActor::PickFirstSolidVoxel(const FVector& LocalStartPosition, const FVector& LocalDirection, FVector& VoxelLocation) const
{
	// TODO: 
	return false;
}

bool AVActor::PickLastSolidVoxel(const FVector& LocalStartPosition, const FVector& LocalDirection, FVector& VoxelLocation) const
{
	// TODO: 
	return false;
}