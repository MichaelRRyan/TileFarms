#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include "TileType.h"

class Tile
{
public:

	TileType m_type;
	sf::IntRect m_textureRect;

	void setType(TileType t_type);
	void setType(TileType t_type, sf::IntRect t_textureRect);

};

#endif // !TILE_H