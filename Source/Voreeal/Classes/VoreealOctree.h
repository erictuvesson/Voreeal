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

	/// The current height of this node.
	int32 m_height;

	FTimespan m_dataLastModified;
	FTimespan m_lastSceduledForUpdate;
	FTimespan m_meshLastChanged;

	/// Constructor
	FSparseOctreeNode(FVoreealRegion Region, int32 ParentId, FSparseOctree* Root);

	/// Gets if the mesh is up to date.
	FORCEINLINE bool IsUpToDate() const 
	{ 
		return (m_meshLastChanged > m_dataLastModified); 
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
	FSparseOctreeNode* GetNodeAt(int32 Index);

	/// Gets the octree region.
	FVoreealRegion GetRegion() const;

	/// Gets the max height of the octree.
	int32 GetMaxHeight() const;

	/// Gets the node count.
	int32 GetCount() const;

	/// Validate the octree
	/// \return bottom node count
	int32 Validate();

	/// Treverse the nodes.
	/// \result return 'ETraverseOptions', parameter 'FSparseOctreeNode*'
	template <typename TCallback>
	inline void Traverse(TCallback && Callback) {
		Traverse(m_rootId, Callback);
	}

	/// Treverse the node.
	/// \result return 'ETraverseOptions', parameter 'FSparseOctreeNode*'
	template <typename TCallback>
	void Traverse(int32 TargetNodeId, TCallback && Callback);

	/// Mark a change at the position.
	void MarkChange(const FIntVector& Position, const FTimespan& ChangeTime);

	/// Mark a change in the region.
	void MarkChange(const FVoreealRegion& Region, const FTimespan& ChangeTime);
	
private:
	/// Create a new node.
	int32 CreateNode(FVoreealRegion Region, int32 ParentId);

	/// Build all parent nodes.
	void BuildNode(int32 ParentId);

	/// Mark a change in the region.
	void MarkChange(const int32& Index, const FVoreealRegion& Region, const FTimespan& ChangeTime);

private:
	UVoreealVolumeComponent* m_volumeComponent;
	UVoreealVolume* m_volume;

	// Octree
	TArray<FSparseOctreeNode*> m_children;
	int32 m_rootId, m_maxHeight, m_baseNodeSize;
	FVoreealRegion m_bounds;
	EOctreeConstructionModes m_constMode;
};

template <typename TCallback>
void FSparseOctree::Traverse(int32 TargetNodeId, TCallback && Callback)
{
	if (TargetNodeId == FSparseOctreeNode::InvalidNodeIndex)
	{
		return;
	}

	int32 StackCount = 1;
	TArray<int32> Stack;

	Stack.Add(TargetNodeId);

	while (StackCount > 0)
	{
		int32 NodeId = Stack[--StackCount];
		Stack.RemoveAt(StackCount);

		FSparseOctreeNode* Node = GetNodeAt(NodeId);

		ETraverseOptions TraverseOptions = Callback(Node);
		if (TraverseOptions == ETraverseOptions::Stop)
			break;

		if (TraverseOptions == ETraverseOptions::Continue && Node->m_hasChildren)
		{
			for (int i = 0; i < FSparseOctreeNode::ChildrenCount; ++i)
			{
				if (Node->m_childrenId[i] != FSparseOctreeNode::InvalidNodeIndex)
				{
					Stack.Add(Node->m_childrenId[i]);
					StackCount++;
				}
			}
		}
	}
}
