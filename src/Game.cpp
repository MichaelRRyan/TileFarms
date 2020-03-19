/// @Author Michael Rainsford Ryan

#include "..\include\Game.h"

///////////////////////////////////////////////////////////////////
Game::Game() :
	m_window{ sf::VideoMode{ Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT, 32u }, Globals::GAME_TITLE },
	m_exitGame{ false },
	m_player{ m_world },
	m_fullScreen{ false }
{
	sf::View view = m_window.getDefaultView();
	view.setSize(view.getSize() / Globals::VIEW_SCALE);
	view.setCenter(view.getCenter() / Globals::VIEW_SCALE);
	m_window.setView(view);

	m_window.setVerticalSyncEnabled(true);

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
		else if (sf::Event::KeyPressed == nextEvent.type)
		{
			if (sf::Keyboard::F11 == nextEvent.key.code)
			{
				if (m_fullScreen) // Switch to full screen
				{
					m_window.create(sf::VideoMode{ Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT, 32u }, Globals::GAME_TITLE);
				}
				else // Switch out of full screen
				{
					m_window.create(sf::VideoMode::getDesktopMode(), Globals::GAME_TITLE, sf::Style::Fullscreen);
				}
				
				// Reset the view
				sf::View view = m_window.getDefaultView();
				view.setSize(view.getSize() / Globals::VIEW_SCALE);
				view.setCenter(view.getCenter() / Globals::VIEW_SCALE);
				m_window.setView(view);

				// Toggle the full screen bool
				m_fullScreen = !m_fullScreen;
			}
#ifdef _DEBUG
			else if (sf::Keyboard::R == nextEvent.key.code)
			{
				WorldGenerator::generateWorld(m_world);
				m_player.setup();
			}
#endif // _DEBUG
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
	m_player.setView(m_window);
}

///////////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear();

	unsigned startY = (m_window.getView().getCenter().y - m_window.getView().getSize().y / 2.0f) / Globals::TILE_SIZE;
	unsigned endY = startY + (m_window.getView().getSize().y / Globals::TILE_SIZE) + 4;

	if (endY > Globals::WORLD_WIDTH_Y)
	{
		endY = Globals::WORLD_WIDTH_Y;
	}

	for (unsigned y = startY; y < endY; y++)
	{
		for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++)
		{
			m_world.drawColumn(m_window, y, z);

			if (m_player.getHeight() == z && m_player.getY() == y)
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
	WorldGenerator::generateWorld(m_world);
	m_player.setup();
}

///////////////////////////////////////////////////////////////////