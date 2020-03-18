#include "World.h"

World::World()
{
	initialise();
	loadTextures();
}

void World::drawColumn(sf::RenderWindow& t_window, unsigned t_y, unsigned t_z)
{
	unsigned startX = (t_window.getView().getCenter().x - t_window.getView().getSize().x / 2.0f) / Globals::TILE_SIZE;
	unsigned endX = startX + (t_window.getView().getSize().x / Globals::TILE_SIZE) + 2;

	if (endX > Globals::WORLD_WIDTH_X)
	{
		endX = Globals::WORLD_WIDTH_X ;
	}

	for (unsigned x = startX; x < endX; x++)
	{
		if (TileType::Null != m_tiles[t_z][t_y][x].m_type) // If the tile to be rendered is not null
		{
			if (t_z + 1 < Globals::WORLD_HEIGHT && TileType::Null == m_tiles[t_z + 1][t_y][x].m_type) // If the tile is not covered
			{
				m_tileSprite.setTextureRect(m_tiles[t_z][t_y][x].m_textureRect);

				// Set the tile sprite's pixel position
				m_tileSprite.setPosition(x * Globals::TILE_SIZE, t_y * Globals::TILE_SIZE + Globals::TILE_SIZE - m_tileSprite.getTextureRect().height);

#ifdef GENERATOR_DEBUG
				sf::Uint8 value{ static_cast<sf::Uint8>(1.0 * t_z / Globals::WORLD_HEIGHT * 155.0 + 100.0) };
				m_tileSprite.setColor(sf::Color{ value, value, value });
#endif // GENERATOR_DEBUG

				// Draw the tile sprite
				t_window.draw(m_tileSprite);
			}
		}
	}
}

TileType const World::getTileType(unsigned t_x, unsigned t_y, unsigned t_z) const
{
	return m_tiles[t_z][t_y][t_x].m_type;
}

void World::setTile(TileType t_tileType, int t_x, int t_y, int t_z)
{
	m_tiles[t_z][t_y][t_x].setType(t_tileType);
}

void World::setTile(TileType t_tileType, sf::IntRect t_textureRect, int t_x, int t_y, int t_z)
{
	m_tiles[t_z][t_y][t_x].setType(t_tileType, t_textureRect);
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
				m_tiles[z][y][x].setType(TileType::Null);
			}
		}
	}
}

void World::generate()
{
	// Generate world
	for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++)
	{
		for (unsigned y = 0; y < Globals::WORLD_WIDTH_Y; y++)
		{
			for (unsigned x = 0; x < Globals::WORLD_WIDTH_X; x++)
			{
				if (0 == z)
				{
					m_tiles[z][y][x].setType(TileType::Grass);
				}
				else if (1 == z)
				{
					if (rand() % 5 == 0)
					{
						m_tiles[z][y][x].setType(TileType::Rock);
					}
					else if (rand() % 20 == 0)
					{
						m_tiles[z][y][x].setType(TileType::Grass);

						if (y + 1 < Globals::WORLD_WIDTH_Y)
						{
							m_tiles[z][y + 1][x].setType(TileType::Slope);
						}
					}
				}
				else
				{
					if (rand() % 20 == 0)
					{
						m_tiles[z][y][x].setType(TileType::Grass);
						m_tiles[z - 1][y][x].setType(TileType::Grass);

						if (y + 1 < Globals::WORLD_WIDTH_Y)
						{
							m_tiles[z][y + 1][x].setType(TileType::Slope);
							m_tiles[z - 1][y + 1][x].setType(TileType::Grass);
						}

						if (y + 2 < Globals::WORLD_WIDTH_Y)
						{
							m_tiles[z - 1][y + 2][x].setType(TileType::Slope);
						}
					}
				}
			}
		}
	}

	m_tiles[1][8][8].setType(TileType::Null);
	m_tiles[2][8][8].setType(TileType::Null);
}
