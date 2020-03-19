#include "Chicken.h"

Chicken::Chicken(World& t_world) :
	m_world{ t_world },
	m_DEFAULT_MOVE_SPEED{ 0.5f },
	m_moveSpeed{ m_DEFAULT_MOVE_SPEED },
	m_height{ 1 },
	m_velocity{ 0.0f, 0.0f },
	m_animationSpeed{ 10.0f }
{
	loadTextures();
}

Chicken::Chicken(World& t_world, sf::Vector2f t_pixelPos, unsigned t_height) :
	m_world{ t_world },
	m_DEFAULT_MOVE_SPEED{ 1.0f },
	m_moveSpeed{ m_DEFAULT_MOVE_SPEED },
	m_height{ t_height },
	m_velocity{ 0.0f, 0.0f },
	m_animationSpeed{ 10.0f }
{
	loadTextures();

	m_sprite.setPosition(t_pixelPos);
}

void Chicken::update()
{
	// Random chance to do something
	if (rand() % 10 == 0)
	{
		if (rand() % 2 == 0)
		{
			m_velocity = { 0.0f, 0.0f }; // stop moving
		}
		else // Move
		{
			sf::Vector2f directionVector{ static_cast<float>(rand() % 3 - 1), static_cast<float>(rand() % 3 - 1) };

			if (vmath::length(directionVector) != 0.0f)
			{
				m_velocity = vmath::unitVector(directionVector) * m_moveSpeed;
			}
		}
	}
	
	if (vmath::length(m_velocity) != 0.0f)
	{
		// Horisontal collisions
		float targetTileX = (m_sprite.getPosition().x + m_velocity.x + (Globals::TILE_SIZE / 3.0f * vmath::sign(m_velocity.x))) / Globals::TILE_SIZE;

		if (targetTileX < 0.0f || targetTileX >= Globals::WORLD_WIDTH_X
			|| m_world.getTileType(static_cast<unsigned>(targetTileX), m_sprite.getPosition().y / Globals::TILE_SIZE, m_height) != TileType::Null)
		{
			m_velocity.x = 0.0f;
		}

		// Vertical collisions
		float targetTileY = (m_sprite.getPosition().y + m_velocity.y + (2.0f * vmath::sign(m_velocity.y))) / Globals::TILE_SIZE;

		if (targetTileY < 0.0f || targetTileY >= Globals::WORLD_WIDTH_Y
			|| m_world.getTileType(m_sprite.getPosition().x / Globals::TILE_SIZE, static_cast<unsigned>(targetTileY), m_height) != TileType::Null)
		{
			m_velocity.y = 0.0f;
		}
	}
	else // If not moving
	{
		m_animationClock.restart();
	}

	m_sprite.move(m_velocity);

	animate();

	if (m_height > 0 && m_world.getTileType(getX(), getY(), m_height - 1) == TileType::Null)
	{
		m_height--;
		std::cout << "Chicken fell. Level: " << m_height << std::endl;
	}
}

void Chicken::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_shadow);
	t_window.draw(m_sprite);
}

unsigned const Chicken::getHeight() const
{
	return m_height;
}

unsigned const Chicken::getY() const
{
	return static_cast<unsigned>(m_sprite.getPosition().y / Globals::TILE_SIZE);
}

unsigned const Chicken::getX() const
{
	return static_cast<unsigned>(m_sprite.getPosition().x / Globals::TILE_SIZE);
}

sf::Vector2f const& Chicken::getPixelPosition() const
{
	return m_sprite.getPosition();
}

void Chicken::animate()
{
	int textureDir = m_sprite.getTextureRect().top / 8;

	// Check if moving
	if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
	{
		// Set the texture direction depending on the direction we're facing
		if (m_velocity.x > 0.0f)
		{
			textureDir = 1;
			m_sprite.setScale(1.0f, 1.0f);
			m_shadow.setPosition(m_sprite.getPosition().x - 2, m_sprite.getPosition().y);
		}
		else if (m_velocity.x < 0.0f)
		{
			textureDir = 1;
			m_sprite.setScale(-1.0f, 1.0f);
			m_shadow.setPosition(m_sprite.getPosition().x + 2, m_sprite.getPosition().y);
		}
		else if (m_velocity.y < 0.0f)
		{
			textureDir = 2;
			m_sprite.setScale(1.0f, 1.0f);
			m_shadow.setPosition(m_sprite.getPosition().x - 1, m_sprite.getPosition().y);
		}
		else if (m_velocity.y > 0.0f)
		{
			textureDir = 0;
			m_sprite.setScale(1.0f, 1.0f);
			m_shadow.setPosition(m_sprite.getPosition().x - 1, m_sprite.getPosition().y);
		}

		int frame = static_cast<int>(m_animationClock.getElapsedTime().asSeconds() * m_moveSpeed * m_animationSpeed) % 4;

		m_sprite.setTextureRect({ frame * 8, textureDir * 8, 8, 8 });
	}
	else // If not moving
	{
		m_sprite.setTextureRect({ 0, textureDir * 8, 8, 8 });
	}
}

void Chicken::loadTextures()
{
	if (!m_spriteSheet.loadFromFile("ASSETS/IMAGES/chicken.png"))
	{
		throw("Error loading chicken texture");
	}

	m_sprite.setTexture(m_spriteSheet);

	m_sprite.setTextureRect({ 0, 0, 8, 8 });
	m_sprite.setOrigin(4.0f, 8.0f);

	m_shadow.setTexture(m_spriteSheet);
	m_shadow.setTextureRect({ 0, 24, 12, 5 });
	m_shadow.setOrigin(6.0f, 3.0f);
}
