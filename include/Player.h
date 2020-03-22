#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "World.h"
#include "VectorMath.h"
#include "Gamepad.h"

class Player
{
public:

	Player(World & t_world);

	void update();

	void draw(sf::RenderWindow & t_window) const;
	void drawInventory(sf::RenderWindow& t_window);

	unsigned const getHeight() const;
	unsigned const getY() const;
	unsigned const getX() const;

	sf::Vector2f const & getPixelPosition() const;

	void setup();

	void setView(sf::RenderWindow & m_window);

private:

	sf::Vector2f const getInputVector() const;

	void handleInput();
	void handleSprintEvent();
	void handleMovement(sf::Vector2f const& t_inputVector);
	bool handleClimbEvent();
	void handleDestroyEvent();
	void handleBuildEvent();
	void handleInventoryEvents();

	void animate();

	void loadTextures();

	void loadFont();

	void updateInventoryText();

	sf::Texture m_spriteSheet;
	sf::Sprite m_sprite;

	World & m_world;

	float const m_DEFAULT_MOVE_SPEED;
	float m_moveSpeed;

	unsigned m_height; // Height in tiles

	sf::Vector2f m_velocity;

	sf::Clock m_animationClock;
	float m_animationSpeed;

	int m_characterNumber; // Character number in sprite sheet

	enum class State
	{
		Walking,
		Climbing,
		Falling
	} m_state;

	sf::Vector2i m_direction;

	sf::Vector2f m_previousPosition;
	sf::Vector2f m_targetPosition;

	Gamepad m_controller;

	std::map<TileType, int> m_inventory;
	TileType m_selectedTile;

	sf::Font m_font;
	sf::Text m_inventoryText;
};

#endif // !PLAYER_H