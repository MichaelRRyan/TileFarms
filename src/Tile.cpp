#include "Tile.h"

void Tile::setType(TileType t_type)
{
	m_type = t_type;

	int leftOffset; // Needs initialisation before the switch statement

	switch (m_type)
	{
	case TileType::Grass:
		leftOffset = (rand() % 3) * 16; // Pick a random grass tile from the three
		m_textureRect = { leftOffset, 64, 16, 16 };
		break;
	case TileType::Rock:
		m_textureRect = { 0, 80, 16, 32 };
		break;
	case TileType::Slope:
		m_textureRect = { 16, 160, 16, 16 };
		break;
	case TileType::Water:
		m_textureRect = { 64, 144, 16, 16 };
		break;
	default:
		m_textureRect = { 0, 0, 0, 0 };
		break;
	}
}

void Tile::setType(TileType t_type, sf::IntRect t_textureRect)
{
	m_type = t_type;

	m_textureRect = t_textureRect;
}
