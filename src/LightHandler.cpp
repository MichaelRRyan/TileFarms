#include "LightHandler.h"

///////////////////////////////////////////////////////////////////
LightHandler::LightHandler() :
	m_colours{ { 
		{ 010.0f, 080.0f, 220.0f }, // Midnight
		{ 010.0f, 070.0f, 200.0f }, // Late night
		{ 010.0f, 080.0f, 220.0f }, // Dawn
		{ 220.0f, 200.0f, 175.0f }, // Morning
		{ 245.0f, 235.0f, 190.0f }, // Noon
		{ 255.0f, 250.0f, 230.0f }, // Late afternoon
		{ 250.0f, 240.0f, 200.0f }, // Dusk
		{ 215.0f, 150.0f, 165.0f } // Early night
		} },
	m_timeNames{ {
			"Midnight",
			"Late night",
			"Dawn",
			"Morning",
			"Noon",
			"Late afternoon",
			"Dusk",
			"Early night"
		}}
{
	loadFonts();
	loadShaders();
}

///////////////////////////////////////////////////////////////////
void LightHandler::update()
{
	// Get the (game) hours since the game started, plus a start hour (9am)
	float timeSinceStart = m_gameTimer.getElapsedTime().asSeconds() / 30 + 9;

	// Get the hour of day refined to 24 hours
	int timeOfDay = static_cast<int>(timeSinceStart) % 24;

	// Get the current timestep (Current colour value of the ambient light)
	int timestep = timeOfDay / 3;

	// Get the colour at the current time step and the next time step
	sf::Vector3f prevColor = m_colours.at(timestep);
	sf::Vector3f nextColor = m_colours.at((timestep + 1) % m_colours.size());

	// Find the blend value of the two colours
	float blend = (timeSinceStart / 3.0f) - floor(timeSinceStart / 3.0f); // Remove the whole number to get a value between 0 and 1

	// Get the blend of the two colours from the blend value
	sf::Vector3f currColor = prevColor + ((nextColor - prevColor) * blend);

	// Get a colour value between 0 and 1 opposed to 0 to 255
	currColor = currColor / 255.0f;

	// Set the ambient light variable in the shader
	m_shader.setUniform("ambientLight", currColor);

	// Find the time in 12 hour format
	if (timeOfDay == 0)
	{
		m_timeText.setString("12 am " + m_timeNames.at(timestep));
	}
	else if (timeOfDay < 12)
	{
		m_timeText.setString(std::to_string(timeOfDay) + " am " + m_timeNames.at(timestep));
	}
	else if (timeOfDay == 12)
	{
		m_timeText.setString("12 pm " + m_timeNames.at(timestep));
	}
	else
	{
		m_timeText.setString(std::to_string(timeOfDay - 12) + " pm " + m_timeNames.at(timestep));
	}
	
	// Set the origin of the text object to keep it centered
	m_timeText.setOrigin(m_timeText.getGlobalBounds().width / 2.0f, 0.0f);
}

///////////////////////////////////////////////////////////////////
void LightHandler::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_timeText);
}

///////////////////////////////////////////////////////////////////
sf::Shader * LightHandler::getShader()
{
	return &m_shader;
}

///////////////////////////////////////////////////////////////////
void LightHandler::loadShaders()
{
	if (!m_shader.loadFromFile("shaders/vertex_shader.vert", "shaders/fragment_shader.frag"))
	{
		std::cout << "Failed to load shader" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////
void LightHandler::loadFonts()
{
	if (!m_font.loadFromFile("ASSETS/FONTS/arial.ttf"))
	{
		throw("Error loading arial font file in player");
	}

	m_timeText.setFont(m_font);
	m_timeText.setPosition(Globals::WINDOW_WIDTH / 2.0f, 0.0f);
}

///////////////////////////////////////////////////////////////////