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

	unsigned const getHeight() const;
	unsigned const getRow() const;

private:

	void handleMovement();

	void animate();

	void loadTextures();

	sf::Texture m_spriteSheet;
	sf::Sprite m_sprite;

	World & m_world;

	float m_moveSpeed;

	unsigned m_height; // Height in tiles

	sf::Vector2f m_velocity;

	sf::Clock m_animationClock;
	float m_animationSpeed;

	int m_characterNumber; // Character number in sprite sheet
};

#endif // !PLAYER_H