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
			int z = (int)(fPerlinNoise2D[y * nOutputWidth + x] * Globals::WORLD_HEIGHT - 1);

			for (int curZ = z; curZ >= 0; curZ--)
			{
				t_world.setTile(TileType::Grass, x, y, curZ);
			}
		}
	}

	removeNoise(t_world);

	addSlopes(t_world);
}

sf::Vector2i removeNoiseTile(World & t_world, int t_x, int t_y, int t_z)
{
	sf::Vector2i connectedTile{ -1, -1 }; // Used to keep a reference to the indices of any connected tiles

	// Check each direction for connected tiles
	// Left
	if (t_x - 1 >= 0 && TileType::Null != t_world.getTileType(t_x - 1, t_y, t_z))
	{
		// No need to check for a previous connected tile as this is the first check (see next if statement)
		connectedTile = { t_x - 1, t_y };
	}
	// Right
	if (t_x + 1 < Globals::WORLD_WIDTH_X && TileType::Null != t_world.getTileType(t_x + 1, t_y, t_z))
	{
		if (connectedTile.x != -1) // If there's another connected tile, continue to the next iteration
		{
			return { -1, -1 };
		}

		connectedTile = { t_x + 1, t_y };
}
	// Up
	if (t_y - 1 >= 0 && TileType::Null != t_world.getTileType(t_x, t_y - 1, t_z))
	{
		if (connectedTile.x != -1) // If there's another connected tile, continue to the next iteration
		{
			return { -1, -1 };
		}

		connectedTile = { t_x, t_y - 1 };
	}
	// Down
	if (t_y + 1 < Globals::WORLD_WIDTH_Y && TileType::Null != t_world.getTileType(t_x, t_y + 1, t_z))
	{
		if (connectedTile.x != -1) // If there's another connected tile, continue to the next iteration
		{
			return { -1, -1 };
		}

		connectedTile = { t_x, t_y + 1 };
	}

	// If we get here without continuing to the next iteration,
	// the tile is not touching two other tiles, and should be removed
	t_world.setTile(TileType::Null, t_x, t_y, t_z);

	return connectedTile;
}

void WorldGenerator::removeNoise(World& t_world)
{
#ifdef GENERATOR_DEBUG
	sf::RenderWindow window{ sf::VideoMode{ 800u, 600u, 32u }, "Window" };

	sf::RectangleShape shape{ { Globals::TILE_SIZE, Globals::TILE_SIZE } };
	shape.setFillColor(sf::Color::Transparent);

	shape.setOutlineColor(sf::Color{ 255, 0, 0, 255 });
	shape.setOutlineThickness(2.0f);
#endif // GENERATOR_DEBUG

	// Clear up small bumps
	for (int z = Globals::WORLD_HEIGHT - 1; z >= 0; z--) // Loop from the heighest level downards
	{
		for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++) // Loop from the northmost point to the southmost
		{
			for (int x = 0; x < Globals::WORLD_WIDTH_X; x++) // Loop from the westmost point to the eastmost
			{
				if (TileType::Null != t_world.getTileType(x, y, z)) // If the current tile is not null
				{
					sf::Vector2i nextTile = removeNoiseTile(t_world, x, y, z);

					while (-1 != nextTile.x)
					{
#ifdef GENERATOR_DEBUG
						window.clear();

						for (int z = 0; z < Globals::WORLD_HEIGHT; z++)
						{
							for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++)
							{
								t_world.drawColumn(window, y, z);
							}
						}

						shape.setPosition(nextTile.x * Globals::TILE_SIZE, nextTile.y * Globals::TILE_SIZE);
						window.draw(shape);

						window.display();

						while (window.isOpen())
						{
							bool nextIteration = false;
							sf::Event sfEvent;

							while (window.pollEvent(sfEvent))
							{
								if (sf::Event::Closed == sfEvent.type)
								{
									window.close();
								}
								else if (sf::Event::KeyPressed == sfEvent.type)
								{
									nextIteration = true;
									break;
								}
							}

							if (nextIteration)
							{
								break;
							}
						}
#endif // GENERATOR_DEBUG

						nextTile = removeNoiseTile(t_world, nextTile.x, nextTile.y, z);
					}
				}
			}
		}
	}
}

void WorldGenerator::addSlopes(World& t_world)
{
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
