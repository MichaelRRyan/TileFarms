#include "Tile.h"

void Tile::setType(TileType t_type)
{
	m_type = t_type;

	switch (m_type)
	{
	case TileType::Grass:
		m_textureRect = { 0, 64, 16, 16 };
		break;
	case TileType::Rock:
		m_textureRect = { 0, 80, 16, 32 };
		break;
	case TileType::Slope:
		m_textureRect = { 16, 160, 16, 16 };
		break;
	default:
		m_textureRect = { 0, 0, 0, 0, };
		break;
	}
}

void Tile::setType(TileType t_type, sf::IntRect t_textureRect)
{
	m_type = t_type;

	m_textureRect = t_textureRect;
}
