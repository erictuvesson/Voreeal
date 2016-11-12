#pragma once

#include "VoreealWorker.h"
#include "VoreealMeshExtractors.h"
#include "VoreealVolumeComponent.generated.h"

UCLASS(Abstract)
class VOREEAL_API UVoreealVolumeComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UVoreealVolumeComponent(const class FObjectInitializer& ObjectInitializer);

protected:
	bool FindFinishedTask(TSharedPtr<FVoreealMesh>& Result);
	void AddTask(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options);

private:
	uint32 Identifier;

	static FVoreealWorker* Runnable;
	static void InitWorker();
};
