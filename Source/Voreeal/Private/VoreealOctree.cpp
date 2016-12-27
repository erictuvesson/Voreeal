#include "VoreealPrivatePCH.h"
#include "VoreealOctree.h"

#include "VoreealVolume.h"
#include "VoreealBasicVolume.h"
#include "VoreealVolumeComponent.h"

FSparseOctreeNode::FSparseOctreeNode(FVoreealRegion region, int32 parentId, FSparseOctree* root)
	: m_selfId(InvalidNodeIndex)
	, m_parentId(parentId)
	, m_hasChildren(false)
	, m_root(root)
	, m_bounds(region)
	, m_height(0)
	, m_dataLastModified(0)
	, m_lastSceduledForUpdate(0)
	, m_meshLastChanged(0)
	, m_bTaskRunning(false)
{
	for (int32 i = 0; i < ChildrenCount; i++)
	{
		m_childrenId[i] = InvalidNodeIndex;
	}

	m_dataLastModified = FTimespan(0, 0, FPlatformTime::Seconds());
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
	, m_baseNodeSize(32)
	, m_maxHeight(2)
{
	if (ConstMode == EOctreeConstructionModes::BoundVoxels)
	{
		m_bounds.ShiftUpperCorner(1, 1, 1);
	}
	else if (ConstMode == EOctreeConstructionModes::BoundCells)
	{
		m_bounds.ShiftUpperCorner(-1, -1, -1);
		m_bounds.ShiftUpperCorner(1, 1, 1);
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

	m_maxHeight = ::PolyVox::logBase2(octreeTargetSize / m_baseNodeSize) - 1;

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

	octreeRegion.Grow(widthInc / 2, heightInc / 2, depthInc / 2);

	m_rootId = CreateNode(octreeRegion, FSparseOctreeNode::InvalidNodeIndex);

	BuildNode(m_rootId);

	//UE_LOG(LogTemp, Warning, TEXT("Octree: Child Nodes %d"), Validate());
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

int32 FSparseOctree::GetMaxHeight() const
{
	return m_maxHeight;
}

int32 FSparseOctree::GetCount() const
{
	return m_children.Num();
}

int32 FSparseOctree::Validate()
{
	int32 Count = 0;

	Traverse([&Count](FSparseOctreeNode* node) -> ETraverseOptions
	{
		if (node->m_hasChildren)
		{
			return ETraverseOptions::Continue;
		}

		Count++;

		return ETraverseOptions::Continue;
	});

	return Count;
}

void FSparseOctree::MarkChangeNow(const FIntVector& Position)
{
	MarkChange(Position, FTimespan(0, 0, FPlatformTime::Seconds()));
}

void FSparseOctree::MarkChangeNow(const FVoreealRegion& Region)
{
	MarkChange(Region, FTimespan(0, 0, FPlatformTime::Seconds()));
}

void FSparseOctree::MarkChange(const FIntVector& position, const FTimespan& changeTime)
{
	MarkChange(m_rootId, FVoreealRegion(position.X, position.Y, position.Z, 1, 1, 1), changeTime);
}

void FSparseOctree::MarkChange(const FVoreealRegion& region, const FTimespan& changeTime)
{
	MarkChange(m_rootId, region, changeTime);
}

int32 FSparseOctree::CreateNode(FVoreealRegion region, int32 parent)
{
	auto node = new FSparseOctreeNode(region, parent, this);

	if (parent != FSparseOctreeNode::InvalidNodeIndex)
	{
		node->m_height = m_children[parent]->m_height + 1;
	}

	m_children.Add(node);

	int32 index = m_children.Num() - 1;
	m_children[index]->m_selfId = index;

	return index;
}

void FSparseOctree::BuildNode(int32 parentId)
{
	FSparseOctreeNode* ParentNode = m_children[parentId];
	FVector ParentNodeSize = ParentNode->m_bounds.Size();

	int32 ParentSize = (m_constMode == EOctreeConstructionModes::BoundCells) 
		? ParentNodeSize.X
		: ParentNodeSize.X + 1;

	if (ParentSize > m_baseNodeSize&&
		ParentNode->m_height < m_maxHeight)
	{
		int32 ChildSize = (m_constMode == EOctreeConstructionModes::BoundCells) 
			? (ParentNodeSize.X) / 2
			: (ParentNodeSize.X + 1) / 2;

		FIntVector Min = ParentNode->m_bounds.GetLowerInt();
		FIntVector Max = (m_constMode == EOctreeConstructionModes::BoundCells)
			? Min + FIntVector(ChildSize,	  ChildSize,	 ChildSize)
			: Min + FIntVector(ChildSize - 1, ChildSize - 1, ChildSize - 1);

		FIntVector Center = (Min + Max) / 2;

		for (int32 i = 0; i < FSparseOctreeNode::ChildrenCount; i++)
		{
			FVoreealRegion ChildRegion;
			ChildRegion.X = (i % 2 == 0 ? Min.X : Center.X);
			ChildRegion.Y = ((i < 2 || i == 4 || i == 5) ? Min.Y : Center.Y);
			ChildRegion.Z = (i < 4 ? Min.Z : Center.Z);
			ChildRegion.Width  = (i % 2 == 0 ? Center.X : Max.X);
			ChildRegion.Height = ((i < 2 || i == 4 || i == 5) ? Center.Y : Max.Y);
			ChildRegion.Depth  = (i < 4 ? Center.Z : Max.Z);

			if (FVoreealRegion::Intersect(ChildRegion, m_bounds))
			{
				int32 Node = CreateNode(ChildRegion, parentId);
				ParentNode->m_childrenId[i] = Node;
				ParentNode->m_hasChildren = true;

				BuildNode(Node);
			}
		}
	}
}

void FSparseOctree::MarkChange(const int32& Index, const FVoreealRegion& Region, const FTimespan& ChangeTime)
{
	SCOPE_CYCLE_COUNTER(STAT_OctreeChange);

	FSparseOctreeNode* Node = m_children[Index];

	if (FVoreealRegion::Intersect(Node->m_bounds, Region))
	{
		Node->m_dataLastModified = ChangeTime;

		if (Node->m_hasChildren)
		{
			for (int32 i = 0; i < FSparseOctreeNode::ChildrenCount; i++)
			{
				int32 ChildIndex = Node->m_childrenId[i];
				if (ChildIndex != FSparseOctreeNode::InvalidNodeIndex)
				{
					MarkChange(Index, Region, ChangeTime);
				}
			}
		}
	}
}
