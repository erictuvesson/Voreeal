#include "VoreealPrivatePCH.h"
#include "VoreealOctree.h"

#include "VoreealVolume.h"
#include "VoreealBasicVolume.h"
#include "VoreealVolumeComponent.h"
#include "VoreealBlueprintLibrary.h"

FSparseOctreeNode::FSparseOctreeNode(FVoreealRegion region, int32 parentId, FSparseOctree* root)
	: m_selfId(InvalidNodeIndex)
	, m_parentId(parentId)
	, m_hasChildren(false)
	, m_root(root)
	, m_bounds(region)
	, m_depth(InvalidNodeIndex)
{
	for (int32 i = 0; i < ChildrenCount; i++)
	{
		m_childrenId[i] = InvalidNodeIndex;
	}
}

FSparseOctree::FSparseOctree(UBasicVolume* Volume, UVoreealVolumeComponent* VolumeComponent,
	const EOctreeConstructionModes& ConstMode)
	: FSparseOctree(Volume, VolumeComponent, Volume->GetEnclosingRegion(), ConstMode)
{

}

FSparseOctree::FSparseOctree(UVoreealVolume* Volume, UVoreealVolumeComponent* VolumeComponent,
	const FVoreealRegion& Region, const EOctreeConstructionModes& ConstMode)
	: m_rootId(FSparseOctreeNode::InvalidNodeIndex)
	, m_bounds(Region)
	, m_volume(m_volume)
	, m_volumeComponent(VolumeComponent)
	, m_constMode(ConstMode)
{
	if (ConstMode == EOctreeConstructionModes::BoundVoxels)
	{
		UVoreealBlueprintLibrary::ShiftUpperCorner(m_bounds, 1, 1, 1, m_bounds);
	}
	else if (ConstMode == EOctreeConstructionModes::BoundCells)
	{
		UVoreealBlueprintLibrary::ShiftUpperCorner(m_bounds, -1, -1, -1, m_bounds);
		UVoreealBlueprintLibrary::ShiftUpperCorner(m_bounds, 1, 1, 1, m_bounds);
	}

	int32 width = (ConstMode == EOctreeConstructionModes::BoundCells)  ? m_bounds.Width  : m_bounds.Width + 1;
	int32 height = (ConstMode == EOctreeConstructionModes::BoundCells) ? m_bounds.Height : m_bounds.Height + 1;
	int32 depth = (ConstMode == EOctreeConstructionModes::BoundCells)  ? m_bounds.Depth  : m_bounds.Depth + 1;

	int32 largestDimension = (std::max)(Region.Width, (std::max)(Region.Height, Region.Depth));
	if (ConstMode == EOctreeConstructionModes::BoundCells)
	{
		largestDimension--;
	}

	int32 octreeTargetSize = ::PolyVox::upperPowerOfTwo(largestDimension);

	m_maxDepth = ::PolyVox::logBase2((octreeTargetSize) / 2/*mBaseNodeSize*/) - 1;

	int32 widthInc = octreeTargetSize - width;
	int32 heightInc = octreeTargetSize - height;
	int32 depthInc = octreeTargetSize - depth;

	FVoreealRegion octreeRegion(m_bounds);

	if (widthInc % 2 == 1)
	{
		octreeRegion.X++;
		octreeRegion.Width -= 2;
		widthInc--;
	}

	if (heightInc % 2 == 1)
	{
		octreeRegion.Y++;
		octreeRegion.Height -= 2;
		heightInc--;
	}

	if (depthInc % 2 == 1)
	{
		octreeRegion.Z++;
		octreeRegion.Depth -= 2;
		depthInc--;
	}

	UVoreealBlueprintLibrary::Grow(octreeRegion, widthInc / 2, heightInc / 2, depthInc / 2, octreeRegion);

	m_rootId = CreateNode(octreeRegion, FSparseOctreeNode::InvalidNodeIndex);
	m_children[m_rootId]->m_depth = m_maxDepth - 1;

	BuildNode(m_rootId);
}

FSparseOctree::~FSparseOctree()
{
	for (int32 i = 0; i < m_children.Num(); i++)
	{
		delete m_children[i];
	}
}

FSparseOctreeNode* FSparseOctree::GetRoot() const
{
	return m_children[m_rootId];
}

FSparseOctreeNode* FSparseOctree::GetNodeAt(int32 index)
{
	return m_children[index];
}

FVoreealRegion FSparseOctree::GetRegion() const
{
	return m_bounds;
}

int32 FSparseOctree::GetMaxDepth() const
{
	return m_maxDepth;
}

int32 FSparseOctree::GetCount() const
{
	return m_children.Num();
}

void FSparseOctree::MarkChange(const FIntVector& position, const FTimespan& changeTime)
{
	MarkChange(m_rootId, FVoreealRegion(position.X, position.Y, position.Z, 1, 1, 1), changeTime);
}

void FSparseOctree::MarkChange(const FVoreealRegion& region, const FTimespan& changeTime)
{
	MarkChange(m_rootId, region, changeTime);
}

bool FSparseOctree::Update(const FVector& viewPosition)
{
	// Traverse all the nodes and see if they need to be updated,
	// if so we queue a task.
	Traverse([=](FSparseOctreeNode* node) -> ETraverseOptions
	{
		if (node->m_hasChildren)
		{
			return ETraverseOptions::Continue;
		}

		if (!node->IsUpToDate() && !node->IsSceduled() && !node->IsTaskRunning())
		{
			//void* extractor = nullptr;

			node->m_lastSceduledForUpdate = FTimespan(0, 0, FPlatformTime::Seconds());
			node->m_lastSurfaceExtractionTask = MakeShareable(new FSparseOctreeTask
			{
				Async<int>(EAsyncExecution::ThreadPool, [/*&extractor, */&node]
				{
					return 1234;
				})
			});

			m_tasks.Add(node->m_lastSurfaceExtractionTask);

			UE_LOG(LogTemp, Warning, TEXT("Voreeal: Queue Task!"));
		}

		return ETraverseOptions::Skip;
	});

	for (int32 i = 0; i < m_tasks.Num();)
	{
		auto task = m_tasks[i];
		if (task->Task.IsReady())
		{
			//task.->m_node->m_lastSurfaceExtractionTask = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Voreeal: Task Complete!"));
			m_tasks.RemoveAt(i, 1, false);
		}
		else
		{
			i++;
		}
	}

	return true;
}

int32 FSparseOctree::CreateNode(FVoreealRegion region, int32 parent)
{
	auto node = new FSparseOctreeNode(region, parent, this);

	if (parent != FSparseOctreeNode::InvalidNodeIndex)
	{
		node->m_depth = m_children[parent]->m_depth + 1;
	}

	m_children.Add(node);

	int32 index = m_children.Num() - 1;
	m_children[index]->m_selfId = index;

	return index;
}

void FSparseOctree::BuildNode(int32 parentId)
{
	int32 parentSize = (m_constMode == EOctreeConstructionModes::BoundCells) 
		? m_children[parentId]->m_bounds.Width
		: m_children[parentId]->m_bounds.Width + 1;

	if (parentSize > 16 /*mBaseNodeSize*/)
	{
		int32 childSize = (m_constMode == EOctreeConstructionModes::BoundCells) 
			? (m_children[parentId]->m_bounds.Width) / 2
			: (m_children[parentId]->m_bounds.Width + 1) / 2;

		FIntVector min = m_children[parentId]->m_bounds.GetLower();
		FIntVector max = (m_constMode == EOctreeConstructionModes::BoundCells)
			? min + FIntVector(childSize, childSize, childSize)
			: min + FIntVector(childSize - 1, childSize - 1, childSize - 1);

		FIntVector center = (min.Size() > max.Size()) ? min - max : max - min;

		for (int i = 0; i < FSparseOctreeNode::ChildrenCount; i++)
		{
			FVoreealRegion childRegion;
			childRegion.X = (i % 2 == 0 ? min.X : center.X);
			childRegion.Y = ((i < 2 || i == 4 || i == 5) ? min.Y : center.Y);
			childRegion.Z = (i < 4 ? min.Z : center.Z);
			childRegion.Width = (i % 2 == 0 ? center.X : max.X);
			childRegion.Height = ((i < 2 || i == 4 || i == 5) ? center.Y : max.Y);
			childRegion.Depth = (i < 4 ? center.Z : max.Z);

			if (UVoreealBlueprintLibrary::Intersect(childRegion, m_bounds))
			{
				int32 node = CreateNode(childRegion, parentId);
				m_children[parentId]->m_childrenId[i] = node;
				m_children[parentId]->m_hasChildren = true;

				BuildNode(node);
			}
		}
	}
}

void FSparseOctree::MarkChange(const int32& index, const FVoreealRegion& region, const FTimespan& changeTime)
{
	FSparseOctreeNode* node = m_children[index];

	if (UVoreealBlueprintLibrary::Intersect(node->m_bounds, region))
	{
		node->m_dataLastModified = changeTime;

		if (node->m_hasChildren)
		{
			for (int32 i = 0; i < FSparseOctreeNode::ChildrenCount; i++)
			{
				int32 childIndex = node->m_childrenId[i];
				if (childIndex != FSparseOctreeNode::InvalidNodeIndex)
				{
					MarkChange(index, region, changeTime);
				}
			}
		}
	}
}
