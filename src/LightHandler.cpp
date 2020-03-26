#include "LightHandler.h"

///////////////////////////////////////////////////////////////////
LightHandler::LightHandler() :
	m_colours{ { 
		{ 010.0f, 070.0f, 200.0f }, // Midnight
		{ 010.0f, 080.0f, 220.0f }, // Late night
		{ 220.0f, 200.0f, 175.0f }, // Dawn
		{ 245.0f, 235.0f, 190.0f }, // Morning
		{ 255.0f, 250.0f, 230.0f }, // Noon
		{ 250.0f, 240.0f, 200.0f }, // Late afternoon
		{ 215.0f, 150.0f, 165.0f }, // Dusk
		{ 010.0f, 080.0f, 220.0f } // Early night
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
	float time = m_gameTimer.getElapsedTime().asSeconds() / 50;

	int timestep = static_cast<int>(time + 3.0f) % m_colours.size();

	sf::Vector3f prevColor = m_colours.at(timestep);
	sf::Vector3f nextColor = m_colours.at((timestep + 1) % m_colours.size());

	float blend = time - floor(time); // Remove the whole number to get a value between 0 and 1

	sf::Vector3f currColor = prevColor + ((nextColor - prevColor) * blend);

	currColor = currColor / 255.0f;

	m_shader.setUniform("ambientLight", currColor);

	// Find the time in 12 hour format
	if (timestep == 0)
	{
		m_timeText.setString("12 am " + m_timeNames.at(timestep));
	}
	else if (timestep * 3 < 12)
	{
		m_timeText.setString(std::to_string(timestep * 3) + " am " + m_timeNames.at(timestep));
	}
	else if (timestep * 3 == 12)
	{
		m_timeText.setString("12 pm " + m_timeNames.at(timestep));
	}
	else
	{
		m_timeText.setString(std::to_string(timestep * 3 - 12) + " pm " + m_timeNames.at(timestep));
	}

	m_timeText.setString(m_timeText.getString() + "\n" + std::to_string(floor(m_gameTimer.getElapsedTime().asSeconds())));
	
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