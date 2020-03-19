#include "Tile.h"

void Tile::setType(TileType t_type)
{
	m_type = t_type;

	int leftOffset; // Needs initialisation before the switch statement

	switch (m_type)
	{
	case TileType::Grass:
		leftOffset = (rand() % 9 % 6) * 16; // Pick a random grass tile from the six (% a second time to add double chance of normal grass opposed to flowers)
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
	case TileType::WaterLily:
		leftOffset = (rand() % 4 + 1) * 16; // Pick a random lily tile out of the four
		m_textureRect = { leftOffset, 80, 16, 16 };
		break;
	case TileType::Bush:
		leftOffset = (rand() % 3 + 4) * 16; // Pick a random bush tile out of the three
		m_textureRect = { leftOffset, 48, 16, 16 };
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
