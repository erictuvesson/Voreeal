#pragma once

#include "VoreealRegion.h"

#include <Future.h>
#include <PolyVox/RawVolume.h>

class UVoreealVolumeComponent;
class UVoreealVolume;
class UBasicVolume;

class FNodeTask;
class FSparseOctree;

/** Sparse Octree Voxel Bounds */
enum class EOctreeConstructionModes
{
	/// Generate only mesh for voxels inside bounds.
	BoundVoxels = 0,
	/// Generate all the voxel sides too.
	BoundCells = 1
};

/** Determines how to traverse the next node when traversing a space partition tree. */
enum class ETraverseOptions
{
	/// The traverse operation should continue to visit the next node.
	Continue,
	/// The traverse operation should skip the current node and its child nodes.
	Skip,
	/// The traverse operation should stop visiting nodes.
	Stop,
};

struct FSparseOctreeTask
{
	TFuture<int> Task;
};

/** Sparse Octree Node */
struct FSparseOctreeNode
{
	static const int32 InvalidNodeIndex = 0xFFFF;
	static const int32 ChildrenCount = 8;

	/// The self node id.
	int32 m_selfId;

	/// The parent node id.
	int32 m_parentId;

	/// The nodes children.
	int32 m_childrenId[ChildrenCount];

	/// Does the node have children.
	uint8 m_hasChildren : 1;

	/// The owning octree.
	FSparseOctree* m_root;

	/// The bounds of this node.
	FVoreealRegion m_bounds;

	/// The current depth of this node. (this is in reverse)
	int32 m_depth;

	/// Extraction Task.
	TSharedPtr<FSparseOctreeTask> m_lastSurfaceExtractionTask;

	FTimespan m_dataLastModified;
	FTimespan m_lastSceduledForUpdate;
	FTimespan m_meshLastChanged;
	//FTimespan m_nodeOrChildrenLastChanged;

	/// Constructor
	FSparseOctreeNode(FVoreealRegion region, int32 parentId, FSparseOctree* root);

	/// Gets if the mesh is up to date.
	FORCEINLINE bool IsUpToDate() const 
	{ 
		return m_meshLastChanged > m_dataLastModified; 
	}

	/// Gets if the mesh is sceduled for update.
	FORCEINLINE bool IsSceduled() const 
	{ 
		return (m_lastSceduledForUpdate > m_dataLastModified) 
			&& (m_lastSceduledForUpdate > m_meshLastChanged);
	}

	/// Gets if extraction mesh generation task is running.
	FORCEINLINE bool IsTaskRunning() const
	{
		return false;
	}
};

/**
 * Sparse Octree, generates meshes for every node and sends them to the 
 * procedural mesh component in sections. The sections can then be set visible 
 * depending on if they are visible from behind other nodes.
 */
class FSparseOctree
{
public:
	/// Constructor
	FSparseOctree(UBasicVolume* Volume, UVoreealVolumeComponent* VolumeComponent,
		const EOctreeConstructionModes& ConstMode);

	/// Constructor
	FSparseOctree(UVoreealVolume* Volume, UVoreealVolumeComponent* VolumeComponent,
		const FVoreealRegion& Region, const EOctreeConstructionModes& ConstMode);

	/// Destructor
	virtual ~FSparseOctree();

	/// Gets the root node.
	FSparseOctreeNode* GetRoot() const;

	/// Gets the node from index.
	FSparseOctreeNode* GetNodeAt(int32 index);

	/// Gets the octree region.
	FVoreealRegion GetRegion() const;

	/// Gets the max depth of the octree.
	int32 GetMaxDepth() const;

	/// Gets the node count.
	int32 GetCount() const;

	/// Treverse the nodes.
	/// @result return 'ETraverseOptions', parameter 'FSparseOctreeNode*'
	template <typename TCallback>
	inline void Traverse(TCallback && result) {
		Traverse(m_rootId, result);
	}

	/// Treverse the node.
	/// @result return 'ETraverseOptions', parameter 'FSparseOctreeNode*'
	template <typename TCallback>
	void Traverse(int32 targetNodeId, TCallback && result);

	/// Mark a change at the position.
	void MarkChange(const FIntVector& position, const FTimespan& changeTime);

	/// Mark a change in the region.
	void MarkChange(const FVoreealRegion& region, const FTimespan& changeTime);
	
	/// Update the octree from the players viewpoint. 
	/// \return true if up to date
	bool Update(const FVector& viewPosition);

private:
	/// Create a new node.
	int32 CreateNode(FVoreealRegion region, int32 parentId);

	/// Build all parent nodes.
	void BuildNode(int32 parentId);

	/// Mark a change in the region.
	void MarkChange(const int32& index, const FVoreealRegion& region, const FTimespan& changeTime);

private:
	UVoreealVolumeComponent* m_volumeComponent;
	UVoreealVolume* m_volume;
	TArray<TSharedPtr<FSparseOctreeTask>> m_tasks;

	// Octree
	TArray<FSparseOctreeNode*> m_children;
	int32 m_rootId, m_maxDepth;
	FVoreealRegion m_bounds;
	EOctreeConstructionModes m_constMode;
};

template <typename TCallback>
void FSparseOctree::Traverse(int32 targetNodeId, TCallback && result)
{
	if (targetNodeId == FSparseOctreeNode::InvalidNodeIndex)
	{
		return;
	}

	int32 stackCount = 1;
	TArray<int32> stack;

	stack.Add(targetNodeId);

	while (stackCount > 0)
	{
		int32 nodeId = stack[--stackCount];
		FSparseOctreeNode* node = GetNodeAt(nodeId);

		ETraverseOptions traverseOptions = result(node);
		if (traverseOptions == ETraverseOptions::Stop)
			break;

		if (traverseOptions == ETraverseOptions::Continue && node->m_hasChildren)
		{
			for (int i = 0; i < FSparseOctreeNode::ChildrenCount; ++i)
			{
				if (node->m_childrenId[i] != FSparseOctreeNode::InvalidNodeIndex)
				{
					stack.Add(node->m_childrenId[i]);
					stackCount++;
				}
			}
		}
	}
}
