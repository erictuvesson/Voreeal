#pragma once

#include "VoreealWorker.h"
#include "VoreealVolumeComponent.generated.h"

UCLASS(Abstract)
class VOREEAL_API UVoreealVolumeComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Voreeal")
	class UProceduralMeshComponent* MeshComponent;

public:
	UVoreealVolumeComponent(const class FObjectInitializer& ObjectInitializer);

protected:
	// Get if there are any finished tasks.
	bool FindFinishedTask(TSharedPtr<FVoreealMesh>& Result);

	// Queue a new task.
	void AddTask(UVoreealVolume* Volume, const FVoreealExtractorOptions& Options);

private:
	uint32 Identifier;

	static FVoreealWorker* Runnable;
	static void InitWorker();

	// TODO: Fix
	friend class UVoreealBlueprintLibrary;
};
