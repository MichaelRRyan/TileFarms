#ifndef GAME
#define GAME

/// @Author Michael Rainsford Ryan

#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "World.h"
#include "Player.h"
#include "WorldGenerator.h"

/// <summary>
/// The game's main class. Keeps the game running at 60
/// frames per second and holds all ground level game objects.
/// </summary>
class Game
{
public:

	Game();
	~Game();

	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void setupShapes();

	sf::RenderWindow m_window;
	bool m_exitGame;

	World m_world;

	Player m_player;

	bool m_fullScreen;

};

#endif // !GAME

