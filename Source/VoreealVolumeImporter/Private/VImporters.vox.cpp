#include "VoreealVolumeImporterPrivatePCH.h"
#include "VImporters.h"

namespace Voreeal
{
	int MV_ID(int a, int b, int c, int d)
	{
		return (a) | (b << 8) | (c << 16) | (d << 24);
	}

	struct vox_chunk_t
	{
		int id;
		int contentSize;
		int childrenSize;
	};

	class MV_RGBA
	{
	public:
		unsigned char r, g, b, a;
	};

	class MV_Voxel
	{
	public:
		unsigned char x, y, z, colorIndex;
	};

	void ReadChunk(FArchive& Ar, vox_chunk_t &chunk)
	{
		// read chunk
		Ar << chunk.id
			<< chunk.contentSize
			<< chunk.childrenSize;

		//// print chunk info
		//const char *c = (const char *)(&chunk.id);
		//printf("[Log] MV_VoxelModel :: Chunk : %c%c%c%c : %d %d\n",
		//	c[0], c[1], c[2], c[3],
		//	chunk.contentSize, chunk.childrenSize
		//	);
	}

	UBasicVolume::VolumeType* ImportMagicaVoxel(FArchive& Ar)
	{
		// https://voxel.codeplex.com/wikipage?title=Sample%20Codes
		//auto* Result = new RawVolume(region);
		//return Result;

		const int MV_VERSION = 150;
		const int ID_VOX = MV_ID('V', 'O', 'X', ' ');
		const int ID_MAIN = MV_ID('M', 'A', 'I', 'N');
		const int ID_SIZE = MV_ID('S', 'I', 'Z', 'E');
		const int ID_XYZI = MV_ID('X', 'Y', 'Z', 'I');
		const int ID_RGBA = MV_ID('R', 'G', 'B', 'A');

		int32 magic;
		int32 version;

		// magic number
		Ar << magic;
		if (magic != ID_VOX)
		{
			UE_LOG(LogVoreealVolumeImporter, Warning, TEXT("magic number does not match"));
			return nullptr;
		}

		// version
		Ar << version;
		if (version != MV_VERSION)
		{
			UE_LOG(LogVoreealVolumeImporter, Warning, TEXT("version does not match"));
			return nullptr;
		}

		// main chunk
		vox_chunk_t mainChunk;
		ReadChunk(Ar, mainChunk);
		if (mainChunk.id != ID_MAIN)
		{
			UE_LOG(LogVoreealVolumeImporter, Warning, TEXT("main chunk is not found"));
			return nullptr;
		}

		bool isCustomPalette;
		MV_RGBA palette[256];

		MV_Voxel* voxels = nullptr;

		int32 sizex = 0, sizey = 0, sizez = 0, numVoxels = 0;
		while (!Ar.AtEnd())
		{
			// read chunk header
			vox_chunk_t sub;
			ReadChunk(Ar, sub);

			if (sub.id == ID_SIZE)
			{
				// size
				Ar << sizex << sizey << sizez;
			}
			else if (sub.id == ID_XYZI)
			{
				// numVoxels
				Ar << numVoxels;
				if (numVoxels < 0)
				{
					UE_LOG(LogVoreealVolumeImporter, Warning, TEXT("negative number of voxels"));
					return nullptr;
				}

				// voxels
				if (numVoxels > 0)
				{
					voxels = new MV_Voxel[numVoxels];
					Ar.Serialize(voxels, sizeof(MV_Voxel) * numVoxels);
				}
			}
			else if (sub.id == ID_RGBA)
			{
				// last color is not used, so we only need to read 255 colors
				isCustomPalette = true;
				Ar.Serialize(palette + 1, sizeof(MV_RGBA) * 255);

				// NOTICE : skip the last reserved color
				MV_RGBA reserved;
				Ar.Serialize(&reserved, sizeof(MV_RGBA));
			}
		}

		PolyVox::Region region;
		region.setLowerCorner(PolyVox::Vector3DInt32(0, 0, 0));
		region.setUpperCorner(PolyVox::Vector3DInt32(sizex, sizey, sizez));

		auto* Result = new UBasicVolume::VolumeType(region);

		for (int32 i = 0; i < numVoxels; i++)
		{
			auto voxel = voxels[i];
			auto color = palette[voxel.colorIndex];

			uint32 data =
				(color.r) |
				(color.g) << 8 |
				(color.b) << 16 |
				(color.a) << 24;

			Result->setVoxel(voxel.x, voxel.y, voxel.z, data);
		}

		delete[] voxels;

		//// print model info
		//printf("[Log] MV_VoxelModel :: Model : %d %d %d : %d\n",
		//	sizex, sizey, sizez, numVoxels
		//	);

		return Result;
	}
}
