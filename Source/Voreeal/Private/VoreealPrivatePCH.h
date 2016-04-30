#pragma once
#pragma warning(disable:4610)

// Unreal
#include <CoreUObject.h>
#include <Engine.h>
#include <DrawDebugHelpers.h>
#include <ProceduralMeshComponent.h>

// Module
#include "VoreealModule.h"

#include "VVector.h"
#include "VRegion.h"
#include "VBaseVolume.h"

#include <PolyVox/CubicSurfaceExtractor.h>
#include <PolyVox/MarchingCubesSurfaceExtractor.h>


class FMeshUpdateTask
{
public:
	UProceduralMeshComponent* MeshComponent;
	FProceduralMesh Mesh;

	FMeshUpdateTask(UProceduralMeshComponent* MeshComponent, FProceduralMesh Mesh)
		: MeshComponent(MeshComponent)
		, Mesh(Mesh)
	{ }

	FORCEINLINE static TStatId GetStatId() { RETURN_QUICK_DECLARE_CYCLE_STAT(FMeshUpdateTask, STATGROUP_TaskGraphTasks); }
	static const TCHAR* GetTaskName() { return TEXT("FMeshUpdateTask"); }
	static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::GameThread; }
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::FireAndForget; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// TODO: !!!
	}
};


template<typename VolumeType>
void Internal_ExtractMesh(VolumeType* Volume, const EVolumeExtractType& Type, const FRegion& Region, const int32& LOD, FProceduralMesh& Mesh)
{
	// Convert Region
	PolyVox::Region region = (PolyVox::Region)Region;

	// Calculate and fix LOD
	VolumeType* processVolume = Volume;
/* // TODO: 
	if (LOD != 0)
	{
		FRegion newRegion = Region;

		// Loop until we find the best possible LOD
		for (int32 i = 0; i < LOD; i++)
		{
			// Check if valid
			newRegion = / 2;
		}

		PolyVox::Region targetRegion = Region.ToPolyVoxRegion();
		PolyVox::Region lodRegion = newRegion.ToPolyVoxRegion();

		processVolume = new UBasicVolume::RawVolume(lodRegion);

		PolyVox::VolumeResampler<VolumeType, UBasicVolume::RawVolume*> volumeResampler(Volume, targetRegion, processVolume, lodRegion);
		volumeResampler.execute();
	}
*/
	// Extract Mesh
	switch (Type)
	{
	case EVolumeExtractType::ColoredCubes:
	{
		auto mesh = extractCubicMesh(processVolume, region);
		ConvertMesh(mesh, Mesh);
	}
	case EVolumeExtractType::MarchingCubes:
	{
		auto mesh = extractMarchingCubesMesh(processVolume, region);
		ConvertMesh(mesh, Mesh);
	}
	}

	if (LOD != 0)
	{
		// Make sure this is safe
		delete processVolume;
	}

	//UE_LOG(LogVoreeal, Log, TEXT("[%s] vertices: %d, indices: %d"),
	//	*this->GetName(), Mesh.Vertices.Num(), Mesh.Indices.Num());
}

template<typename TMesh>
void ConvertMesh(TMesh& Mesh, FProceduralMesh& OutMesh)
{
	OutMesh = FProceduralMesh();

	OutMesh.Vertices.AddUninitialized(4);
	OutMesh.Normals.AddUninitialized(4);
	OutMesh.UVs.AddUninitialized(4);
	OutMesh.Colors.AddUninitialized(4);
	OutMesh.Tangents.AddUninitialized(4);

	for (uint32 i = 0; i < Mesh.getNoOfIndices() - 2; i += 3)
	{
		// We could scale the mesh here too
		auto index = Mesh.getIndex(i + 2);
		auto vertex2 = Mesh.getVertex(index);

		index = Mesh.getIndex(i + 1);
		auto vertex1 = Mesh.getVertex(index);

		index = Mesh.getIndex(i);
		auto vertex0 = Mesh.getVertex(index);

		auto vertex0Pos = decodePosition(vertex0.encodedPosition);
		auto vertex1Pos = decodePosition(vertex1.encodedPosition);
		auto vertex2Pos = decodePosition(vertex2.encodedPosition);

		OutMesh.Indices.Add(OutMesh.Vertices.Add(FPolyVoxVector(vertex2Pos)));
		OutMesh.Indices.Add(OutMesh.Vertices.Add(FPolyVoxVector(vertex1Pos)));
		OutMesh.Indices.Add(OutMesh.Vertices.Add(FPolyVoxVector(vertex0Pos)));

		const FVector edge01 = FPolyVoxVector(vertex1Pos - vertex0Pos);
		const FVector edge02 = FPolyVoxVector(vertex2Pos - vertex0Pos);

		const FVector tangentX = edge01.GetSafeNormal();
		FVector tangentZ = (edge01 ^ edge02).GetSafeNormal();

		for (int32 i = 0; i < 3; i++)
		{
			OutMesh.Tangents.Add(FProcMeshTangent(tangentX, false));
			OutMesh.Normals.Add(tangentZ);
		}
	}
}
