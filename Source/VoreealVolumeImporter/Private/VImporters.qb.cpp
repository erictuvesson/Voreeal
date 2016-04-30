#include "VoreealVolumeImporterPrivatePCH.h"
#include "VImporters.h"

namespace Voreeal
{
	struct QubicleMatrix
	{
		char nameLength;
		char* name;

		uint32 sizeX;
		uint32 sizeY;
		uint32 sizeZ;

		int posX;
		int posY;
		int posZ;

		uint32 ***data;
	};

	UBasicVolume::VolumeType* ImportQubicleBinary(FArchive& Ar)
	{
		const uint32 VER_1_0_0_0 = 0x0100;
		const uint32 VER_1_1_0_0 = 0x0101;

		const uint32 CODEFLAG = 2;
		const uint32 NEXTSLICEFLAG = 6;

		uint32 version;
		uint32 colorFormat;
		uint32 zAxisOrientation;
		uint32 compressed;
		uint32 visibilityMaskEncoded;
		uint32 numMatrices;

		Ar << version;
		Ar << colorFormat;
		Ar << zAxisOrientation;
		Ar << compressed;
		Ar << visibilityMaskEncoded;
		Ar << numMatrices;

		if (version != VER_1_0_0_0 && version != VER_1_1_0_0)
		{
			UE_LOG(LogVoreealVolumeImporter, Warning, TEXT("Wrong Qubicle version!"));
			return nullptr;
		}

		if (numMatrices == 0)
		{
			UE_LOG(LogVoreealVolumeImporter, Warning, TEXT("Data is invalid. (MatricesCount)"));
			return nullptr;
		}

		QubicleMatrix* matrices = new QubicleMatrix[numMatrices];

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

		// Merge and convert matrices
		if (numMatrices > 0)
		{
			PolyVox::Vector3DInt32 min(0, 0, 0);
			PolyVox::Vector3DInt32 max(0, 0, 0);

			for (uint32 i = 0; i < numMatrices; i++)
			{
				min.setX(std::min(min.getX(), matrices[i].posX));
				min.setY(std::min(min.getY(), matrices[i].posY));
				min.setZ(std::min(min.getZ(), matrices[i].posZ));

				max.setX(std::max(max.getX(), (matrices[i].posX + (int)matrices[i].sizeX)));
				max.setY(std::max(max.getY(), (matrices[i].posY + (int)matrices[i].sizeY)));
				max.setZ(std::max(max.getZ(), (matrices[i].posZ + (int)matrices[i].sizeZ)));
			}

			PolyVox::Region region;
			region.setUpperCorner(PolyVox::Vector3DInt32(max.getX() + 1, max.getY() + 1, max.getZ() + 1));
			region.setLowerCorner(PolyVox::Vector3DInt32(min.getX() - 1, min.getY() - 1, min.getZ() - 1));

			auto* volume = new UBasicVolume::VolumeType(region);

			for (uint32 i = 0; i < numMatrices; i++)
			{
				for (uint32 z = 0; z < matrices[i].sizeZ; z++)
				{
					for (uint32 y = 0; y < matrices[i].sizeY; y++)
					{
						for (uint32 x = 0; x < matrices[i].sizeX; x++)
						{
							uint32 actualX = matrices[i].posX + x;
							uint32 actualY = matrices[i].posY + y;
							uint32 actualZ = matrices[i].posZ + z;

							uint32 encodedData = matrices[i].data[x][y][z];

							//uint8 a = (encodedData & 0xFF000000) >> 24;
							//uint8 b = (encodedData & 0x00FF0000) >> 16;
							//uint8 g = (encodedData & 0x0000FF00) >> 8;
							//uint8 r = (encodedData & 0x000000FF);

							volume->setVoxel(actualX, actualY, actualZ, encodedData);
						}
					}
				}
			}

			return volume;
		}

		return nullptr;
	}
}
