/// @Author Michael Rainsford Ryan

#include "..\include\Game.h"

///////////////////////////////////////////////////////////////////
Game::Game() :
	m_window{ sf::VideoMode{ Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT, 32u }, "Basic Game" },
	m_exitGame{ false },
	m_player{ m_world }
{
	sf::View view = m_window.getDefaultView();
	view.setSize(view.getSize() / Globals::VIEW_SCALE);
	view.setCenter(view.getCenter() / Globals::VIEW_SCALE);
	m_window.setView(view);

	setupShapes();
}

///////////////////////////////////////////////////////////////////
Game::~Game()
{

}

///////////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float FPS = 60.0f;
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // Run as many times as possible
		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // Run at a minimum of 60 fps
			update(timePerFrame); // 60 fps
		}
		render(); // Run as many times as possible
	}
}

///////////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event nextEvent;
	while (m_window.pollEvent(nextEvent))
	{
		if (sf::Event::Closed == nextEvent.type) // check if the close window button is clicked on.
		{
			m_window.close();
		}
	}
}

///////////////////////////////////////////////////////////////////
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	m_player.update();
}

///////////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear();

	for (unsigned y = 0; y < Globals::WORLD_WIDTH_Y; y++)
	{
		for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++)
		{
			m_world.drawColumn(m_window, y, z);

			if (m_player.getHeight() == z && m_player.getRow() == y)
			{
				m_player.draw(m_window);
			}
		}
	}
	
	m_window.display();
}

///////////////////////////////////////////////////////////////////
void Game::setupShapes()
{

}

///////////////////////////////////////////////////////////////////