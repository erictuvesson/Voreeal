#pragma once
#pragma warning(disable:4610)

// Unreal
#include <CoreUObject.h>
#include <Engine.h>
#include <DrawDebugHelpers.h>
#include <ProceduralMeshComponent.h>

// Module
#include "VoreealModule.h"

// Classes
#include "VoreealVector.h"
#include "VoreealRegion.h"
#include "VoreealVolume.h"

// PolyVox Classes
#include <PolyVox/CubicSurfaceExtractor.h>
#include <PolyVox/MarchingCubesSurfaceExtractor.h>
#include <PolyVox/Raycast.h>
#include <PolyVox/Picking.h>

DECLARE_LOG_CATEGORY_EXTERN(LogVoreeal, Log, All);

DECLARE_STATS_GROUP(TEXT("Voreeal"), STATGROUP_Voreeal, STATCAT_Advanced);
DECLARE_CYCLE_STAT_EXTERN(TEXT("OctreeChange"), STAT_OctreeChange, STATGROUP_Voreeal, VOREEAL_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("RequestMesh"), STAT_RequestMesh, STATGROUP_Voreeal, VOREEAL_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("ReturnMesh"), STAT_ReturnMesh, STATGROUP_Voreeal, VOREEAL_API);
