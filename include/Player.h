#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "World.h"

class Player
{
public:

	Player(World & t_world);

	void update();

	void draw(sf::RenderWindow & t_window) const;

private:

	void handleMovement();

	void loadTextures();

	sf::Texture m_spriteSheet;
	sf::Sprite m_sprite;

	World & m_world;

	float m_moveSpeed;

	unsigned m_height; // Height in tiles
};

#endif // !PLAYER_H