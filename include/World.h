#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <array>

#include "TileType.h"
#include "Globals.h"

class World
{
public:

	World();

	void draw(sf::RenderWindow & t_window);

	TileType const getTileType(unsigned t_x, unsigned t_y, unsigned t_z) const;

private:

	void loadTextures();
	void initialise();

	std::array<std::array<std::array<TileType, Globals::WORLD_WIDTH_X>, Globals::WORLD_WIDTH_Y>, Globals::WORLD_HEIGHT> m_tiles;

	sf::Texture m_textureSheet;

	sf::Sprite m_tileSprite;
};

#endif // !WORLD_H