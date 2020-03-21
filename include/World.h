#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <array>

#include "Tile.h"
#include "Globals.h"

class World
{
public:

	World();

	void initialise();

	void drawColumn(sf::RenderWindow& t_window, unsigned t_y, unsigned t_z);

	TileType const getTileType(unsigned t_x, unsigned t_y, unsigned t_z) const;

	void setTile(TileType t_tileType, int t_x, int t_y, int t_z);

	void setTile(TileType t_tileType, sf::IntRect t_textureRect, int t_x, int t_y, int t_z);

	void setEdgeTile(TileType t_replaceType, TileType t_targetType, int x, int y, int z, int t_imageOffsetX, int t_imageOffsetY);

	void updateTile(int x, int y, int z);

	void updateAdjecent(int x, int y, int z);

	void destroyTile(int x, int y, int z);

	void buildTile(TileType t_type, int x, int y, int z);

private:

	void loadTextures();

	std::array<std::array<std::array<Tile, Globals::WORLD_WIDTH_X>, Globals::WORLD_WIDTH_Y>, Globals::WORLD_HEIGHT> m_tiles;

	sf::Texture m_textureSheet;

	sf::Sprite m_tileSprite;
};

#endif // !WORLD_H