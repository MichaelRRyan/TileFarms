#ifndef LIGHT_HANDLER_H
#define LIGHT_HANDLER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include "Globals.h"

class LightHandler
{
public:

	LightHandler();

	void update();

	void draw(sf::RenderWindow & t_window) const;

	sf::Shader * getShader();

private:

	void loadShaders();
	void loadFonts();


	std::array<sf::Vector3f, 8> m_colours;
	std::array<std::string, 8> m_timeNames;

	sf::Shader m_shader;

	sf::Clock m_gameTimer;

	sf::Text m_timeText;
	sf::Font m_font;

};

#endif // !LIGHT_HANDLER_H