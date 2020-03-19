#ifndef CHICKEN_H
#define CHICKEN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "World.h"
#include "VectorMath.h"

class Chicken
{
public:
	Chicken(World& t_world);

	Chicken(World& t_world, sf::Vector2f t_pixelPos, unsigned t_height);

	void update();

	void draw(sf::RenderWindow& t_window) const;

	unsigned const getHeight() const;
	unsigned const getY() const;
	unsigned const getX() const;

	sf::Vector2f const& getPixelPosition() const;

private:

	void animate();

	void loadTextures();

	sf::Texture m_spriteSheet;
	sf::Sprite m_sprite;

	World& m_world;

	float const m_DEFAULT_MOVE_SPEED;
	float m_moveSpeed;

	unsigned m_height; // Height in tiles

	sf::Vector2f m_velocity;

	sf::Clock m_animationClock;
	float m_animationSpeed;

	sf::Sprite m_shadow;
};

#endif // !CHICKEN_H