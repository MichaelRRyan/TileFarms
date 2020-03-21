#include "WorldGenerator.h"

void WorldGenerator::generateWorld(World& t_world)
{
	t_world.initialise(); // Reset the world with null values

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
			int z = (int)(fPerlinNoise2D[y * nOutputWidth + x] * Globals::WORLD_HEIGHT);

			if (z == 0)
			{
				t_world.setTile(TileType::Water, x, y, 0);
			}
			else
			{
				for (int curZ = z - 1; curZ >= 0; curZ--)
				{
					t_world.setTile(TileType::Grass, x, y, curZ);
				}
			}
		}
	}

	removeNoise(t_world);

	addSlopes(t_world);

	addTrees(t_world);

	addPlants(t_world);
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
					t_world.setEdgeTile(TileType::Slope, TileType::Null, x, y, z, 0, 128);
				}
			}
		}
	}

	// Add water corners
	for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++)
	{
		for (int x = 0; x < Globals::WORLD_WIDTH_X; x++)
		{
			if (TileType::Grass == t_world.getTileType(x, y, 0))
			{
				t_world.setEdgeTile(TileType::Grass, TileType::Water, x, y, 0, 48, 128);
			}
		}
	}
}

void WorldGenerator::addTrees(World& t_world)
{
	for (int z = Globals::WORLD_HEIGHT - 1; z >= 1; z--) // Loop from the heighest level downards
	{
		for (int y = 1; y < Globals::WORLD_WIDTH_Y; y++) // Loop from the northmost point to the southmost
		{
			for (int x = 1; x < Globals::WORLD_WIDTH_X - 1; x++) // Loop from the westmost point to the eastmost
			{
				if (TileType::Null == t_world.getTileType(x, y, z)		// If the current tile is null
					&& TileType::Grass == t_world.getTileType(x, y, z - 1)) // And the tile below is grass
				{
					// Check for space on the left and right
					// No need for boundary checking as the loop starts from 1 and ends at WORLD_WIDTH_X - 1
					if (TileType::Null == t_world.getTileType(x - 1, y, z)
						&& TileType::Null == t_world.getTileType(x + 1, y, z)
						&& TileType::Grass == t_world.getTileType(x - 1, y, z - 1)
						&& TileType::Grass == t_world.getTileType(x + 1, y, z - 1))
					{
						// Check behind for space
						if (TileType::Null == t_world.getTileType(x - 1, y - 1, z)
							&& TileType::Null == t_world.getTileType(x, y - 1, z)
							&& TileType::Null == t_world.getTileType(x + 1, y - 1, z)
							&& TileType::Grass == t_world.getTileType(x - 1, y - 1, z - 1)
							&& TileType::Grass == t_world.getTileType(x , y - 1, z - 1)
							&& TileType::Grass == t_world.getTileType(x + 1, y - 1, z - 1))
						{
							// If there's space for a tree, place one 1 in 10 times
							if (rand() % 30 == 0)
							{
								t_world.setTile(TileType::Tree, { 96, 128, 16, 48 }, x - 1, y, z);
								t_world.setTile(TileType::Tree, { 112, 128, 16, 48 }, x, y, z);
								t_world.setTile(TileType::Tree, { 128, 128, 16, 48 }, x + 1, y, z);
							}
						}
					}
				}
			}
		}
	}
}

void WorldGenerator::addPlants(World& t_world)
{
	for (int z = Globals::WORLD_HEIGHT - 1; z >= 1; z--) // Loop from the heighest level downards
	{
		for (int y = 0; y < Globals::WORLD_WIDTH_Y; y++) // Loop from the northmost point to the southmost
		{
			for (int x = 0; x < Globals::WORLD_WIDTH_X; x++) // Loop from the westmost point to the eastmost
			{
				if (TileType::Null == t_world.getTileType(x, y, z)) // Make sure the current tile is null
				{
					if (TileType::Water == t_world.getTileType(x, y, z - 1)) // Check if the tile below is water
					{
						if (rand() % 10 == 0)
						{
							t_world.setTile(TileType::WaterLily, x, y, z);
						}
					}
					else if (TileType::Grass == t_world.getTileType(x, y, z - 1)) // Check if the tile below is water
					{
						if (rand() % 60 == 0)
						{
							t_world.setTile(TileType::Bush, x, y, z);
						}
					}
				}
			}
		}
	}
}
