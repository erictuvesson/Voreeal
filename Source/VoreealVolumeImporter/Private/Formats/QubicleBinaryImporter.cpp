#include "../VoreealVolumeImporterPrivatePCH.h"
#include "QubicleBinaryImporter.h"

class QubicleMatrix
{
public:
	char nameLength;
	char* name;

	uint32 sizeX;
	uint32 sizeY;
	uint32 sizeZ;

	int posX;
	int posY;
	int posZ;

	uint32 ***data;

	~QubicleMatrix()
	{
		if (name) delete[] name;
		if (data)
		{
			for (uint32 x = 0; x < sizeX; x++)
			{
				for (uint32 y = 0; y < sizeY; y++)
				{
					delete[] data[x][y];
				}
				delete data[x];
			}
		}
	}
};

class QubicleModel
{
	const uint32 VER_1_0_0_0 = 0x0100;
	const uint32 VER_1_1_0_0 = 0x0101;

	const uint32 CODEFLAG = 2;
	const uint32 NEXTSLICEFLAG = 6;

public:
	uint32 version;
	uint32 colorFormat;
	uint32 zAxisOrientation;
	uint32 compressed;
	uint32 visibilityMaskEncoded;
	uint32 numMatrices;

	QubicleMatrix* matrices;

public:
	~QubicleModel()
	{
		if (matrices) delete[] matrices;
	}

	bool ReadHeader(FArchive& Ar, FFeedbackContext* Warn)
	{
		Ar << version;
		Ar << colorFormat;
		Ar << zAxisOrientation;
		Ar << compressed;
		Ar << visibilityMaskEncoded;
		Ar << numMatrices;

		if (version != VER_1_0_0_0 &&
			version != VER_1_1_0_0)
		{
			Warn->Logf(ELogVerbosity::Error, TEXT("Wrong Qubicle version!"));
			return false;
		}

		if (numMatrices == 0)
		{
			Warn->Logf(ELogVerbosity::Error, TEXT("Data is invalid. (MatricesCount)"));
			return false;
		}

		return true;
	}

	void ReadData(FArchive& Ar)
	{
		matrices = new QubicleMatrix[numMatrices];

		for (uint32 i = 0; i < numMatrices; i++)
		{
			Ar << matrices[i].nameLength;
			matrices[i].name = new char[matrices[i].nameLength + 1];

			Ar.Serialize(&matrices[i].name[0], matrices[i].nameLength);
			matrices[i].name[matrices[i].nameLength] = 0;

			Ar << matrices[i].sizeX << matrices[i].sizeY << matrices[i].sizeZ;
			Ar << matrices[i].posX << matrices[i].posY << matrices[i].posZ;

			// Create 3D Array
			matrices[i].data = new uint32**[matrices[i].sizeX];
			for (uint32 j = 0; j < matrices[i].sizeX; j++)
			{
				matrices[i].data[j] = new uint32*[matrices[i].sizeY];
				for (uint32 k = 0; k < matrices[i].sizeY; k++)
				{
					matrices[i].data[j][k] = new uint32[matrices[i].sizeZ];
				}
			}

			if (compressed == 0)
			{
				for (uint32 z = 0; z < matrices[i].sizeZ; z++)
				{
					for (uint32 y = 0; y < matrices[i].sizeY; y++)
					{
						for (uint32 x = 0; x < matrices[i].sizeX; x++)
						{
							uint32 data = 0;
							Ar << data;
							matrices[i].data[x][y][z] = data;
						}
					}
				}
			}
			else
			{
				uint32 z = 0;
				while (z < matrices[i].sizeZ)
				{
					uint32 index = 0;
					while (true)
					{
						uint32 data = 0;
						Ar << data;

						if (data == NEXTSLICEFLAG)
							break;
						else if (data == CODEFLAG)
						{
							uint32 count = 0;
							Ar << count;
							Ar << data;

							for (uint32 j = 0; j < count; j++)
							{
								uint32 x = index % matrices[i].sizeX;
								uint32 y = index / matrices[i].sizeX;
								matrices[i].data[x][y][z] = data;
								index++;
							}
						}
						else
						{
							uint32 x = index % matrices[i].sizeX;
							uint32 y = index / matrices[i].sizeX;
							matrices[i].data[x][y][z] = data;
							index++;
						}
					}
					z++;
				}
			}
		}
	}
};

bool FQubicleBinaryImporter::Import(FArchive& Ar, UBasicVolume* Volume, FFeedbackContext* Warn)
{
	QubicleModel model;
	if (!model.ReadHeader(Ar, Warn))
	{
		return false;
	}

	model.ReadData(Ar);

	// Merge and convert matrices
	if (model.numMatrices > 0)
	{
		PolyVox::Vector3DInt32 min(0, 0, 0);
		PolyVox::Vector3DInt32 max(0, 0, 0);

		for (uint32 i = 0; i < model.numMatrices; i++)
		{
			min.setX(std::min(min.getX(), model.matrices[i].posX));
			min.setY(std::min(min.getY(), model.matrices[i].posY));
			min.setZ(std::min(min.getZ(), model.matrices[i].posZ));
			max.setX(std::max(max.getX(), (model.matrices[i].posX + (int)model.matrices[i].sizeX)));
			max.setY(std::max(max.getY(), (model.matrices[i].posY + (int)model.matrices[i].sizeY)));
			max.setZ(std::max(max.getZ(), (model.matrices[i].posZ + (int)model.matrices[i].sizeZ)));
		}

		PolyVox::Region region;
		region.setUpperCorner(PolyVox::Vector3DInt32(max.getX() + 1, max.getY() + 1, max.getZ() + 1));
		region.setLowerCorner(PolyVox::Vector3DInt32(min.getX() - 1, min.getY() - 1, min.getZ() - 1));

		Volume->ResizeRegion(region);

		for (uint32 i = 0; i < model.numMatrices; i++)
		{
			for (uint32 z = 0; z < model.matrices[i].sizeZ; z++)
			{
				for (uint32 y = 0; y < model.matrices[i].sizeY; y++)
				{
					for (uint32 x = 0; x < model.matrices[i].sizeX; x++)
					{
						uint32 actualX = model.matrices[i].posX + x;
						uint32 actualY = model.matrices[i].posY + y;
						uint32 actualZ = model.matrices[i].posZ + z;

						uint32 encodedData = model.matrices[i].data[x][y][z];

						uint8 a = (encodedData & 0xFF000000) >> 24;
						uint8 b = (encodedData & 0x00FF0000) >> 16;
						uint8 g = (encodedData & 0x0000FF00) >> 8;
						uint8 r = (encodedData & 0x000000FF);

						//Volume->SetVoxelXYZ(actualX, actualY, actualZ, FColor(r, g, b, a));
					}
				}
			}
		}

		return true;
	}

	// there are no matrices
	return false;
}
