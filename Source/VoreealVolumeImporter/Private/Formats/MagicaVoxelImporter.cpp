#include "../VoreealVolumeImporterPrivatePCH.h"
#include "MagicaVoxelImporter.h"

// https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt

const uint32 mv_default_palette[256] =
{
	0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
	0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
	0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
	0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
	0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
	0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
	0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
	0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
	0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
	0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
	0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
	0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
	0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
	0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
	0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
	0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
};

class FColorRGBA
{
public:
	uint8 r, g, b, a;
};

class MV_Voxel 
{
public:
	uint8 x, y, z, colorIndex;
};

class MagicaVoxelMaterial
{
public:
	int32 Index;
	int32 MaterialType;
	float MaterialWeight;
	int32 Properties;
	// float * N
};

class MagicaVoxelModel
{
public:
	// size
	int32 sizex = 0;
	int32 sizey = 0;
	int32 sizez = 0;

	// voxels
	int32 numVoxels = 0;
	MV_Voxel* voxels = nullptr;

public:
	~MagicaVoxelModel()
	{
		if (voxels)
		{
			delete[] voxels;
		}
	}
};

class MagicaVoxelFile
{
	static constexpr int32 MV_VERSION = 150;
	static constexpr int32 ID_VOX_ = 0x20584F56;
	static constexpr int32 ID_MAIN = 0x4E49414D;
	static constexpr int32 ID_SIZE = 0x455A4953;
	static constexpr int32 ID_XYZI = 0x495A5958;
	static constexpr int32 ID_RGBA = 0x41424752;
	static constexpr int32 ID_PACK = 0x4B434150;
	static constexpr int32 ID_MATT = 0x5454414D;

public:
	// version
	int32 Version;
	int32 MagicNumber;

	// palette
	bool bIsCustomPalette = false;
	FColorRGBA Palette[256];

	// Num of SIZE and XYZI chunks
	int32 NumModels = 1;
	TArray<MagicaVoxelModel> Models;

public:
	bool ValidSize()
	{
		for (auto model : Models)
		{
			if (model.sizex <= 0 || model.sizey <= 0 || model.sizez <= 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Load(FArchive& Ar, FFeedbackContext* Warn)
	{
		struct vox_chunk_header {
			int32 id, contentSize, childrenSize;
		};

		Ar << MagicNumber;
		if (MagicNumber != ID_VOX_)
		{
			Warn->Logf(ELogVerbosity::Error, TEXT("Magic number does not match. (%X != %X)"), MagicNumber, ID_VOX_);
			return false;
		}

		Ar << Version;
		if (Version != MV_VERSION)
		{
			Warn->Logf(ELogVerbosity::Error, TEXT("Version does not match. (%d != %d)"), Version, MV_VERSION);
			return false;
		}

		// main chunk
		vox_chunk_header mainChunk;
		Ar << mainChunk.id << mainChunk.contentSize << mainChunk.childrenSize;

		if (mainChunk.id != ID_MAIN)
		{
			Warn->Logf(ELogVerbosity::Error, TEXT("Main chunk was not found."));
			return false;
		}

		MagicaVoxelModel CurrentModel;
		while (!Ar.AtEnd())
		{
			// read chunk header
			vox_chunk_header sub;
			Ar << sub.id << sub.contentSize << sub.childrenSize;

			switch (sub.id)
			{
			case ID_PACK:
			{
				Ar << NumModels;
				break;
			}
			case ID_SIZE: // Read the volume size
			{
				Ar << CurrentModel.sizex << CurrentModel.sizey << CurrentModel.sizez;
				break;
			}
			case ID_XYZI: // Validate the voxel count and read the voxels
			{
				Ar << CurrentModel.numVoxels;

				if (CurrentModel.numVoxels < 0)
				{
					Warn->Logf(ELogVerbosity::Error, TEXT("Negative number of voxels. :("));
					return false;
				}

				CurrentModel.voxels = new MV_Voxel[CurrentModel.numVoxels];
				Ar.Serialize(CurrentModel.voxels, sizeof(MV_Voxel) * CurrentModel.numVoxels);
				
				Models.Add(CurrentModel);
				CurrentModel = MagicaVoxelModel();
				
				break;
			}
			case ID_RGBA: // Read the custom palette
			{
				// last color is not used, so we only need to read 255 colors
				bIsCustomPalette = true;
				Ar.Serialize(Palette + 1, sizeof(FColorRGBA) * 255);

				// NOTICE : skip the last reserved color
				FColorRGBA reserved;
				Ar.Serialize(&reserved, sizeof(FColorRGBA));
				break;
			}
			default:
				// TODO: throw exception
				Ar.Seek(sub.contentSize);
				Ar.Seek(sub.childrenSize);
				break;
			}
		}

		return true;
	}
};

bool FMagicaVoxelImporter::Import(FArchive& Ar, UBasicVolume* Volume, FFeedbackContext* Warn)
{
	MagicaVoxelFile file;
	if (!file.Load(Ar, Warn))
	{
		Warn->Logf(ELogVerbosity::Error, TEXT("Cannot load file header."));
		return false;
	}

	if (!file.ValidSize())
	{
		Warn->Logf(ELogVerbosity::Error, TEXT("Volume has no size."));
		return false;
	}

	//PolyVox::Region region;
	//region.setLowerCorner(PolyVox::Vector3DInt32(0, 0, 0));
	//region.setUpperCorner(PolyVox::Vector3DInt32(file.Models[0].sizex, file.Models[0].sizey, file.Models[0].sizez));

	// TODO: Match models
	MagicaVoxelModel model = file.Models[0];

	Volume->Resize(FIntVector(model.sizex, model.sizey, model.sizez));
	
	for (int32 i = 0; i < model.numVoxels; i++)
	{
		auto voxel = model.voxels[i];

		FColorRGBA rgba;
		if (file.bIsCustomPalette)
		{
			rgba = file.Palette[model.voxels[i].colorIndex];
		}
		else
		{
			unsigned int color = mv_default_palette[model.voxels[i].colorIndex];
			rgba.a = (color >> 24) & 0xFF;
			rgba.b = (color >> 16) & 0xFF;
			rgba.g = (color >> 8) & 0xFF;
			rgba.r = (color >> 0) & 0xFF;
		}

		Volume->SetVoxelXYZ((int32)voxel.x, (int32)voxel.y, (int32)voxel.z, FColor(rgba.r, rgba.g, rgba.b), 255);
	}

	return true;
}
