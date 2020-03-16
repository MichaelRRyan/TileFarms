#include "World.h"

World::World()
{
	initialise();
	loadTextures();
}

void World::draw(sf::RenderWindow& t_window)
{
	for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++)
	{
		for (unsigned y = 0; y < Globals::WORLD_WIDTH_Y; y++)
		{
			for (unsigned x = 0; x < Globals::WORLD_WIDTH_X; x++)
			{
				if (TileType::Null != m_tiles[z][y][x])
				{
					if (TileType::Grass == m_tiles[z][y][x])
					{
						m_tileSprite.setTextureRect({ 0, 64, 16, 16 });
					}
					else if (TileType::Rock == m_tiles[z][y][x])
					{
						m_tileSprite.setTextureRect({ 0, 80, 16, 32 });
					}

					// Set the tile sprite's pixel position
					m_tileSprite.setPosition(x * Globals::TILE_SIZE, y * Globals::TILE_SIZE + Globals::TILE_SIZE - m_tileSprite.getTextureRect().height);

					// Draw the tile sprite
					t_window.draw(m_tileSprite);
				}
			}
		}
	}
}

void World::drawColumn(sf::RenderWindow& t_window, unsigned t_y, unsigned t_z)
{
	for (unsigned x = 0; x < Globals::WORLD_WIDTH_X; x++)
	{
		if (TileType::Null != m_tiles[t_z][t_y][x])
		{
			if (TileType::Grass == m_tiles[t_z][t_y][x])
			{
				m_tileSprite.setTextureRect({ 0, 64, 16, 16 });
			}
			else if (TileType::Rock == m_tiles[t_z][t_y][x])
			{
				m_tileSprite.setTextureRect({ 0, 80, 16, 32 });
			}
			else if (TileType::Slope == m_tiles[t_z][t_y][x])
			{
				m_tileSprite.setTextureRect({ 16, 160, 16, 16 });
			}

			// Set the tile sprite's pixel position
			m_tileSprite.setPosition(x * Globals::TILE_SIZE, t_y * Globals::TILE_SIZE + Globals::TILE_SIZE - m_tileSprite.getTextureRect().height);

			sf::Uint8 lightness{ static_cast<sf::Uint8>(255 - t_z * 50) };
			m_tileSprite.setColor(sf::Color{ lightness, lightness, lightness });

			// Draw the tile sprite
			t_window.draw(m_tileSprite);
		}
	}
}

TileType const World::getTileType(unsigned t_x, unsigned t_y, unsigned t_z) const
{
	return m_tiles[t_z][t_y][t_x];
}

void World::loadTextures()
{
	if (!m_textureSheet.loadFromFile("ASSETS/IMAGES/Tile_sheet.png"))
	{
		throw("Error loading texture sheet");
	}

	m_tileSprite.setTexture(m_textureSheet);
}

void World::initialise()
{
	// Initialise world with all null tiles
	for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++)
	{
		for (unsigned y = 0; y < Globals::WORLD_WIDTH_Y; y++)
		{
			for (unsigned x = 0; x < Globals::WORLD_WIDTH_X; x++)
			{
				m_tiles[z][y][x] = TileType::Null;
			}
		}
	}

	// Generate world
	for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++)
	{
		for (unsigned y = 0; y < Globals::WORLD_WIDTH_Y; y++)
		{
			for (unsigned x = 0; x < Globals::WORLD_WIDTH_X; x++)
			{
				if (0 == z)
				{
					m_tiles[z][y][x] = TileType::Grass;
				}
				else if (1 == z)
				{
					if (rand() % 5 == 0)
					{
						m_tiles[z][y][x] = TileType::Rock;
					}
					else if (rand() % 20 == 0)
					{
						m_tiles[z][y][x] = TileType::Grass;

						if (y + 1 < Globals::WORLD_WIDTH_Y)
						{
							m_tiles[z][y + 1][x] = TileType::Slope;
						}
					}
				}
				else
				{
					if (rand() % 20 == 0)
					{
						m_tiles[z][y][x] = TileType::Grass;
						m_tiles[z - 1][y][x] = TileType::Grass;

						if (y + 1 < Globals::WORLD_WIDTH_Y)
						{
							m_tiles[z][y + 1][x] = TileType::Slope;
							m_tiles[z - 1][y + 1][x] = TileType::Grass;
						}

						if (y + 2 < Globals::WORLD_WIDTH_Y)
						{
							m_tiles[z - 1][y + 2][x] = TileType::Slope;
						}
					}
				}
			}
		}
	}

	m_tiles[1][8][8] = TileType::Null;
	m_tiles[2][8][8] = TileType::Null;
}
