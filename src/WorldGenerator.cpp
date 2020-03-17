#include "WorldGenerator.h"

void WorldGenerator::generateWorld(World& t_world)
{
	// 2D noise variables
	int nOutputWidth = 800;
	int nOutputHeight = 800;
	float* fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
	float* fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];
	for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;

	int nOctaveCount = 7;
	float fScalingBias = 0.2f;

	PerlinNoiseGenerator::PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, nOctaveCount, fScalingBias, fPerlinNoise2D);

	for (int x = 0; x < Globals::WORLD_WIDTH_X; x++)
	{
		for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++)
		{
			int z = (int)(fPerlinNoise2D[y * nOutputWidth + x] * 3.0f);

			for (int curZ = z; curZ >= 0; curZ--)
			{
				t_world.setTile(TileType::Grass, x, y, curZ);
			}
		}
	}

	// Set slopes
	for (int z = Globals::WORLD_HEIGHT - 1; z >= 0; z--)
	{
		for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++)
		{
			for (int x = 0; x < Globals::WORLD_WIDTH_X; x++)
			{
				if (TileType::Grass == t_world.getTileType(x, y, z))
				{
					// Left
					if (x - 1 >= 0 && TileType::Null == t_world.getTileType(x - 1, y, z))
					{
						t_world.setTile(TileType::Slope, x, y, z);
					}
					// Right
					else if (x + 1 < Globals::WORLD_WIDTH_X && TileType::Null == t_world.getTileType(x + 1, y, z))
					{
						t_world.setTile(TileType::Slope, x, y, z);
					}
					// Up
					else if (y - 1 >= 0 && TileType::Null == t_world.getTileType(x, y - 1, z))
					{
						t_world.setTile(TileType::Slope, x, y, z);
					}
					// Down
					else if (y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null == t_world.getTileType(x, y + 1, z))
					{
						t_world.setTile(TileType::Slope, x, y, z);
					}
				}
			}
		}
	}
}
