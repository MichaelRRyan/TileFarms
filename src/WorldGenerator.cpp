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
						// If there's nothing above either
						if (y - 1 >= 0 && TileType::Null == t_world.getTileType(x, y - 1, z))
						{
							t_world.setTile(TileType::Slope, { 0, 128, 16, 16 }, x, y, z);
						}
						// If there's nothing below either
						else if (y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null == t_world.getTileType(x, y + 1, z))
						{
							t_world.setTile(TileType::Slope, { 0, 160, 16, 16 }, x, y, z);
						}
						else
						{
							t_world.setTile(TileType::Slope, { 0, 144, 16, 16 }, x, y, z);
						}
					}
					// Right
					else if (x + 1 < Globals::WORLD_WIDTH_X && TileType::Null == t_world.getTileType(x + 1, y, z))
					{
						// If there's nothing above either
						if (y - 1 >= 0 && TileType::Null == t_world.getTileType(x, y - 1, z))
						{
							t_world.setTile(TileType::Slope, { 32, 128, 16, 16 }, x, y, z);
						}
						// If there's nothing below either
						else if (y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null == t_world.getTileType(x, y + 1, z))
						{
							t_world.setTile(TileType::Slope, { 32, 160, 16, 16 }, x, y, z);
						}
						else
						{
							t_world.setTile(TileType::Slope, { 32, 144, 16, 16 }, x, y, z);
						}
					}
					// Up
					else if (y - 1 >= 0 && TileType::Null == t_world.getTileType(x, y - 1, z))
					{
						t_world.setTile(TileType::Slope, { 16, 128, 16, 16 }, x, y, z);
					}
					// Down
					else if (y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null == t_world.getTileType(x, y + 1, z))
					{
						t_world.setTile(TileType::Slope, { 16, 160, 16, 16 }, x, y, z);
					}
					// Top left
					else if (x - 1 >= 0 && y - 1 >= 0 && TileType::Null == t_world.getTileType(x - 1, y - 1, z))
					{
						t_world.setTile(TileType::Slope, { 16, 192, 16, 16 }, x, y, z);
					}
					// Top Right
					else if (x + 1 < Globals::WORLD_WIDTH_X && y - 1 >= 0 && TileType::Null == t_world.getTileType(x + 1, y - 1, z))
					{
						t_world.setTile(TileType::Slope, { 0, 192, 16, 16 }, x, y, z);
					}
					// Bottom Right
					else if (x + 1 < Globals::WORLD_WIDTH_X && y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null == t_world.getTileType(x + 1, y + 1, z))
					{
						t_world.setTile(TileType::Slope, { 0, 176, 16, 16 }, x, y, z);
					}
					// Bottom Left
					else if (x - 1 >= 0 && y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null == t_world.getTileType(x - 1, y + 1, z))
					{
						t_world.setTile(TileType::Slope, { 16, 176, 16, 16 }, x, y, z);
					}
				}
			}
		}
	}
}
