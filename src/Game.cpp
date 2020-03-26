/// @Author Michael Rainsford Ryan

#include "..\include\Game.h"

///////////////////////////////////////////////////////////////////
Game::Game() :
	m_window{ sf::VideoMode::getDesktopMode(), Globals::GAME_TITLE, sf::Style::Fullscreen },
	m_exitGame{ false },
	m_player{ m_world },
	m_fullScreen{ false },
	m_hasLight{ true }
{
	setWindowState(m_fullScreen);

	m_window.setVerticalSyncEnabled(true);

	resetGame();

#ifdef CINEMATIC_CAMERA
	// Set the camera target location
	cameraTarget.x = rand() % static_cast<int>(Globals::WORLD_WIDTH_X* Globals::TILE_SIZE - m_window.getView().getSize().x) + (m_window.getView().getSize().x / 2.0f);
	cameraTarget.y = rand() % static_cast<int>(Globals::WORLD_WIDTH_Y* Globals::TILE_SIZE - m_window.getView().getSize().y) + (m_window.getView().getSize().y / 2.0f);
#endif
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
				setWindowState(!m_fullScreen);
			}
#ifdef _DEBUG
			else if (sf::Keyboard::R == nextEvent.key.code)
			{
				resetGame();
			}
			else if (sf::Keyboard::Escape == nextEvent.key.code)
			{
				m_window.close();
			}
			else if (sf::Keyboard::C == nextEvent.key.code)
			{
				m_chickens.push_back(new Chicken(m_world, m_player.getPixelPosition(), m_player.getHeight()));
			}
			else if (sf::Keyboard::T == nextEvent.key.code)
			{
				for (int i = 0; i < 8; i++)
				{
					if (sf::Joystick::isConnected(i))
					{
						std::cout << i << std::endl;

						std::string info{ sf::Joystick::getIdentification(0).name + "\n"
							+ std::to_string(sf::Joystick::getIdentification(0).productId) + "\n"
							+ std::to_string(sf::Joystick::getIdentification(0).vendorId) };

						std::cout << info << std::endl;
					}
				}
			}
			else if (sf::Keyboard::O == nextEvent.key.code) // Output to file
			{
				FileHandler::saveWorldToFile(m_world, "TestWorld2");
			}
			else if (sf::Keyboard::I == nextEvent.key.code) // Output to file
			{
				FileHandler::readWorldFromFile(m_world, "TestWorld1");

				m_player.setup();

				for (Chicken* chicken : m_chickens)
				{
					delete chicken;
				}

				m_chickens.clear();
			}
			else if (sf::Keyboard::P == nextEvent.key.code) // Output to file
			{
				std::vector<std::string> fileNames{ FileHandler::getSaveFileNames() };

				std::cout << "File names:" << std::endl;

				for (std::string const& name : fileNames)
				{
					std::cout << " - " << name << std::endl;
				}
			}
			else if (sf::Keyboard::L == nextEvent.key.code)
			{
				m_hasLight = !m_hasLight;
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

	for (Chicken * chicken : m_chickens)
	{
		chicken->update();
	}

	m_player.update();

#ifndef CINEMATIC_CAMERA
	m_player.setView(m_window);
	
#else
	sf::Vector2f vectorToTarget{ cameraTarget - m_window.getView().getCenter() };
	float distanceToTarget{ sqrtf(vectorToTarget.x * vectorToTarget.x + vectorToTarget.y * vectorToTarget.y) };
	sf::Vector2f unitVector = vectorToTarget / distanceToTarget;

	m_window.setView({ m_window.getView().getCenter() + (unitVector / 6.0f), m_window.getView().getSize() });

	if (distanceToTarget < 1.0f)
	{
		// Set the camera pos
		cameraTarget.x = rand() % static_cast<int>(Globals::WORLD_WIDTH_X * Globals::TILE_SIZE - m_window.getView().getSize().x) + (m_window.getView().getSize().x / 2.0f);
		cameraTarget.y = rand() % static_cast<int>(Globals::WORLD_WIDTH_Y * Globals::TILE_SIZE - m_window.getView().getSize().y) + (m_window.getView().getSize().y / 2.0f);

		m_window.setView({ cameraTarget, m_window.getView().getSize() });

		// Set the target pos
		cameraTarget.x = rand() % static_cast<int>(Globals::WORLD_WIDTH_X * Globals::TILE_SIZE - m_window.getView().getSize().x) + (m_window.getView().getSize().x / 2.0f);
		cameraTarget.y = rand() % static_cast<int>(Globals::WORLD_WIDTH_Y * Globals::TILE_SIZE - m_window.getView().getSize().y) + (m_window.getView().getSize().y / 2.0f);

		// Randomise the world 
		resetGame();
}
#endif // CINEMATIC_CAMERA

	m_lightHandler.update();

	// Set shader uniforms
	m_lightHandler.getShader()->setUniform("lightPos", m_player.getPixelPosition());
	m_lightHandler.getShader()->setUniform("hasLight", m_hasLight);
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
	
	for (unsigned z = 0; z < Globals::WORLD_HEIGHT; z++) // Loop from the bottom level to the top
	{
		for (unsigned y = startY; y < endY; y++) // Loop from north to south
		{
			m_world.drawColumn(m_window, m_lightHandler.getShader(), y, z);

#ifndef CINEMATIC_CAMERA
			if (m_player.getHeight() == z && m_player.getY() == y)
			{
				m_player.draw(m_window, m_lightHandler.getShader());
			}
#endif // !CINEMATIC_CAMERA

			for (Chicken const* chicken : m_chickens)
			{
				if (chicken->getHeight() == z && chicken->getY() == y)
				{
					chicken->draw(m_window);
				}
			}
		}
	}

	m_player.drawInventory(m_window);

	sf::View currentView = m_window.getView();
	m_window.setView(m_window.getDefaultView());
	
	m_lightHandler.draw(m_window);

	m_window.setView(currentView);
	
	m_window.display();
}

///////////////////////////////////////////////////////////////////
void Game::resetGame()
{
	WorldGenerator::generateWorld(m_world);
	m_player.setup();

	for (Chicken * chicken : m_chickens)
	{
		delete chicken;
	}

	m_chickens.clear();
}

///////////////////////////////////////////////////////////////////
void Game::setWindowState(bool const t_fullscreen)
{
	if (t_fullscreen) // Switch out of full screen
	{
		m_window.create(sf::VideoMode::getDesktopMode(), Globals::GAME_TITLE, sf::Style::Fullscreen);
	}
	else // Switch to full screen
	{
		m_window.create(sf::VideoMode{ Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT, 32u }, Globals::GAME_TITLE);
	}

	// Reset the view
	sf::View view = m_window.getDefaultView();
	view.setSize(view.getSize() / Globals::VIEW_SCALE);
	view.setCenter(view.getCenter() / Globals::VIEW_SCALE);
	m_window.setView(view);

	// Toggle the full screen bool
	m_fullScreen = t_fullscreen;
}

///////////////////////////////////////////////////////////////////