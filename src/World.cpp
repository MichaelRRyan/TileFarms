#include "World.h"

///////////////////////////////////////////////////////////////////
World::World()
{
	initialise();
	loadTextures();
}

///////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////
void World::drawColumn(sf::RenderWindow& t_window, sf::Shader const& t_shader, unsigned t_y, unsigned t_z)
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
			if (t_z + 1 >= Globals::WORLD_HEIGHT || TileType::Grass != m_tiles[t_z + 1][t_y][x].m_type) // If the tile is not covered or is at the top of the world
			{
				m_tileSprite.setTextureRect(m_tiles[t_z][t_y][x].m_textureRect);

				// Set the tile sprite's pixel position
				m_tileSprite.setPosition(x * Globals::TILE_SIZE, t_y * Globals::TILE_SIZE + Globals::TILE_SIZE - m_tileSprite.getTextureRect().height);

#ifdef GENERATOR_DEBUG
				sf::Uint8 value{ static_cast<sf::Uint8>(1.0 * t_z / Globals::WORLD_HEIGHT * 155.0 + 100.0) };
				m_tileSprite.setColor(sf::Color{ value, value, value });
#endif // GENERATOR_DEBUG

				// Draw the tile sprite
				t_window.draw(m_tileSprite, &t_shader);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
TileType const World::getTileType(unsigned t_x, unsigned t_y, unsigned t_z) const
{
	return m_tiles[t_z][t_y][t_x].m_type;
}

///////////////////////////////////////////////////////////////////
sf::IntRect const World::getTileTextureRect(unsigned t_x, unsigned t_y, unsigned t_z) const
{
	return m_tiles[t_z][t_y][t_x].m_textureRect;
}

///////////////////////////////////////////////////////////////////
void World::setTile(TileType t_tileType, int t_x, int t_y, int t_z)
{
	m_tiles[t_z][t_y][t_x].setType(t_tileType);
}

///////////////////////////////////////////////////////////////////
void World::setTile(TileType t_tileType, sf::IntRect t_textureRect, int t_x, int t_y, int t_z)
{
	m_tiles[t_z][t_y][t_x].setType(t_tileType, t_textureRect);
}

///////////////////////////////////////////////////////////////////
void World::setEdgeTile(TileType t_replaceType, TileType t_targetType, int x, int y, int z, int t_imageOffsetX, int t_imageOffsetY)
{
	// Left
	if (x - 1 >= 0 && t_targetType == getTileType(x - 1, y, z))
	{
		// If there's nothing above either
		if (y - 1 >= 0 && t_targetType == getTileType(x, y - 1, z))
		{
			setTile(t_replaceType, { t_imageOffsetX, t_imageOffsetY, 16, 16 }, x, y, z);
		}
		// If there's nothing below either
		else if (y + 1 < Globals::WORLD_WIDTH_Y && t_targetType == getTileType(x, y + 1, z))
		{
			setTile(t_replaceType, { t_imageOffsetX, t_imageOffsetY + 32, 16, 16 }, x, y, z);
		}
		else
		{
			setTile(t_replaceType, { t_imageOffsetX, t_imageOffsetY + 16, 16, 16 }, x, y, z);
		}
	}
	// Right
	else if (x + 1 < Globals::WORLD_WIDTH_X && t_targetType == getTileType(x + 1, y, z))
	{
		// If there's nothing above either
		if (y - 1 >= 0 && t_targetType == getTileType(x, y - 1, z))
		{
			setTile(t_replaceType, { t_imageOffsetX + 32, t_imageOffsetY, 16, 16 }, x, y, z);
		}
		// If there's nothing below either
		else if (y + 1 < Globals::WORLD_WIDTH_Y && t_targetType == getTileType(x, y + 1, z))
		{
			setTile(t_replaceType, { t_imageOffsetX + 32, t_imageOffsetY + 32, 16, 16 }, x, y, z);
		}
		else
		{
			setTile(t_replaceType, { t_imageOffsetX + 32, t_imageOffsetY + 16, 16, 16 }, x, y, z);
		}
	}
	// Up
	else if (y - 1 >= 0 && t_targetType == getTileType(x, y - 1, z))
	{
		setTile(t_replaceType, { t_imageOffsetX + 16, t_imageOffsetY, 16, 16 }, x, y, z);
	}
	// Down
	else if (y + 1 < Globals::WORLD_WIDTH_Y && t_targetType == getTileType(x, y + 1, z))
	{
		setTile(t_replaceType, { t_imageOffsetX + 16, t_imageOffsetY + 32, 16, 16 }, x, y, z);
	}
	// Top left
	else if (x - 1 >= 0 && y - 1 >= 0 && t_targetType == getTileType(x - 1, y - 1, z))
	{
		setTile(t_replaceType, { t_imageOffsetX + 16, t_imageOffsetY + 64, 16, 16 }, x, y, z);
	}
	// Top Right
	else if (x + 1 < Globals::WORLD_WIDTH_X && y - 1 >= 0 && t_targetType == getTileType(x + 1, y - 1, z))
	{
		setTile(t_replaceType, { t_imageOffsetX, t_imageOffsetY + 64, 16, 16 }, x, y, z);
	}
	// Bottom Right
	else if (x + 1 < Globals::WORLD_WIDTH_X && y + 1 < Globals::WORLD_WIDTH_Y && t_targetType == getTileType(x + 1, y + 1, z))
	{
		setTile(t_replaceType, { t_imageOffsetX, t_imageOffsetY + 48, 16, 16 }, x, y, z);
	}
	// Bottom Left
	else if (x - 1 >= 0 && y + 1 < Globals::WORLD_WIDTH_Y && t_targetType == getTileType(x - 1, y + 1, z))
	{
		setTile(t_replaceType, { t_imageOffsetX + 16, t_imageOffsetY + 48, 16, 16 }, x, y, z);
	}
}

///////////////////////////////////////////////////////////////////
void World::updateTile(int x, int y, int z)
{
	if (TileType::Grass == getTileType(x, y, z)
		|| TileType::Slope == getTileType(x, y, z))
	{
		setTile(TileType::Grass, x, y, z);
		setEdgeTile(TileType::Slope, TileType::Null, x, y, z, 0, 128);
	}
}

///////////////////////////////////////////////////////////////////
void World::updateAdjecent(int x, int y, int z)
{
	if (x - 1 >= 0) // Left
	{
		updateTile(x - 1, y, z);

		if (y - 1 >= 0) // Top left
		{
			updateTile(x - 1, y - 1, z);
		}
		if (y + 1 < Globals::WORLD_WIDTH_Y) // Bottom left
		{
			updateTile(x - 1, y + 1, z);
		}
	}
	if (x + 1 < Globals::WORLD_WIDTH_X) // Right
	{
		updateTile(x + 1, y, z);

		if (y - 1 >= 0) // Top right
		{
			updateTile(x + 1, y - 1, z);
		}
		if (y + 1 < Globals::WORLD_WIDTH_Y) // Bottom left
		{
			updateTile(x + 1, y + 1, z);
		}
	}
	if (y - 1 >= 0) // Top
	{
		updateTile(x, y - 1, z);
	}
	if (y + 1 < Globals::WORLD_WIDTH_Y) // Bottom
	{
		updateTile(x, y + 1, z);
	}
}

///////////////////////////////////////////////////////////////////
void World::destroyTile(int x, int y, int z)
{
	if (TileType::Tree == getTileType(x, y, z))
	{
		setTile(TileType::Null, x, y, z);

		if (x - 1 >= 0 && TileType::Tree == getTileType(x - 1, y, z))
		{
			setTile(TileType::Null, x - 1, y, z);
		}
		else if (x + 2 < Globals::WORLD_WIDTH_X && TileType::Tree == getTileType(x + 2, y, z))
		{
			setTile(TileType::Null, x + 2, y, z);
		}

		if (x + 1 < Globals::WORLD_WIDTH_X && TileType::Tree == getTileType(x + 1, y, z))
		{
			setTile(TileType::Null, x + 1, y, z);
		}
		else if (x - 2 >= 0 && TileType::Tree == getTileType(x - 2, y, z))
		{
			setTile(TileType::Null, x - 2, y, z);
		}

		return;
	}

	setTile(TileType::Null, x, y, z);

	updateAdjecent(x, y, z);
}

///////////////////////////////////////////////////////////////////
void World::buildTile(TileType t_type, int x, int y, int z)
{
	setTile(t_type, x, y, z);
	updateTile(x, y, z);
	updateAdjecent(x, y, z);
}

///////////////////////////////////////////////////////////////////
void World::loadTextures()
{
	if (!m_textureSheet.loadFromFile("ASSETS/IMAGES/Tile_sheet.png"))
	{
		throw("Error loading texture sheet");
	}

	m_tileSprite.setTexture(m_textureSheet);
}

///////////////////////////////////////////////////////////////////