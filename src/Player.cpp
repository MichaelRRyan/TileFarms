#include "Player.h"

Player::Player(World& t_world) :
	m_world{ t_world },
	m_moveSpeed{ 0.5f },
	m_height{ 1 },
	m_velocity{ 0.0f, 0.0f },
	m_characterNumber{ 0 },
	m_animationSpeed{ 10.0f },
	m_DEFAULT_MOVE_SPEED{ 0.5f }
{
	loadTextures();
}

void Player::update()
{
	if (m_height > 0 && m_world.getTileType(getX(), getY(), m_height - 1) == TileType::Null)
	{
		m_height--;
		std::cout << "Fell. Level: " << m_height << std::endl;
	}

	handleMovement();
	animate();
}

void Player::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_sprite);
}

unsigned const Player::getHeight() const
{
	return m_height;
}

unsigned const Player::getY() const
{
	return m_sprite.getPosition().y / Globals::TILE_SIZE;
}

unsigned const Player::getX() const
{
	return m_sprite.getPosition().x / Globals::TILE_SIZE;
}

void Player::setup()
{
	m_sprite.setPosition(Globals::TILE_SIZE * 8.5f, Globals::TILE_SIZE * 8.5f);

	int x = m_sprite.getPosition().x / Globals::TILE_SIZE;
	int y = m_sprite.getPosition().y / Globals::TILE_SIZE;

	for (int z = Globals::WORLD_HEIGHT - 1; z >= 0; z--)
	{
		if (m_world.getTileType(x, y, z) == TileType::Null)
		{
			m_height = z;
		}
		else
		{
			break;
		}
	}
}

void Player::setView(sf::RenderWindow& m_window)
{
	sf::View view = m_window.getView();

	sf::Vector2f vectorToPlayer{ m_sprite.getPosition() - view.getCenter() };

	view.setCenter(view.getCenter() + vectorToPlayer / 20.0f);

	// Left screen boundary
	if (view.getCenter().x - view.getSize().x / 2.0f < 0.0f)
	{
		view.setCenter(view.getSize().x / 2.0f, view.getCenter().y);
	}

	// Top screen boundary
	if (view.getCenter().y - view.getSize().y / 2.0f < 0.0f)
	{
		view.setCenter(view.getCenter().x, view.getSize().y / 2.0f);
	}

	// Right screen boundary
	if (view.getCenter().x + view.getSize().x / 2.0f > Globals::WORLD_WIDTH_X * Globals::TILE_SIZE)
	{
		view.setCenter(Globals::WORLD_WIDTH_X * Globals::TILE_SIZE - view.getSize().x / 2.0f, view.getCenter().y);
	}

	// Bottom screen boundary
	if (view.getCenter().y + view.getSize().y / 2.0f > Globals::WORLD_WIDTH_Y * Globals::TILE_SIZE)
	{
		view.setCenter(view.getCenter().x, Globals::WORLD_WIDTH_Y * Globals::TILE_SIZE - view.getSize().y / 2.0f);
	}


	m_window.setView(view);
}

void Player::handleMovement()
{
	sf::Vector2f inputVector;
	m_velocity = { 0.0f, 0.0f };
	m_moveSpeed = m_DEFAULT_MOVE_SPEED;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		m_moveSpeed = m_DEFAULT_MOVE_SPEED * 3.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		inputVector.x--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		inputVector.x++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		inputVector.y--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		inputVector.y++;
	}

	if (vmath::length(inputVector) != 0.0f)
	{
		sf::Vector2f const movementVector = vmath::unitVector(inputVector) * m_moveSpeed;

		// Horisontal collisions
		float targetTileX = (m_sprite.getPosition().x + movementVector.x + (Globals::TILE_SIZE / 3.0f * vmath::sign(movementVector.x))) / Globals::TILE_SIZE;
		
		if (targetTileX >= 0.0f && targetTileX < Globals::WORLD_WIDTH_X)
		{
			if (m_world.getTileType(static_cast<unsigned>(targetTileX), m_sprite.getPosition().y / Globals::TILE_SIZE, m_height) == TileType::Null)
			{
				m_velocity.x = movementVector.x;
			}
		}

		// Vertical collisions
		float targetTileY = (m_sprite.getPosition().y + movementVector.y + (2.0f * vmath::sign(movementVector.y))) / Globals::TILE_SIZE;

		if (targetTileY >= 0.0f && targetTileY < Globals::WORLD_WIDTH_Y)
		{
			if (m_world.getTileType(m_sprite.getPosition().x / Globals::TILE_SIZE, static_cast<unsigned>(targetTileY), m_height) == TileType::Null)
			{
				m_velocity.y = movementVector.y;
			}
		}

		m_sprite.move(m_velocity);
	}
	else // If not moving
	{
		m_animationClock.restart();
	}
}

void Player::animate()
{
	int textureDir = m_sprite.getTextureRect().top / 32 - m_characterNumber;

	// Check if moving
	if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
	{
		// Set the texture direction depending on the direction we're facing
		if (m_velocity.x > 0.0f)
		{
			textureDir = 1;
			m_sprite.setScale(1.0f, 1.0f);
		}
		else if (m_velocity.x < 0.0f)
		{
			textureDir = 1;
			m_sprite.setScale(-1.0f, 1.0f);
		}
		else if (m_velocity.y < 0.0f)
		{
			textureDir = 2;
		}
		else if (m_velocity.y > 0.0f)
		{
			textureDir = 0;
		}

		int frame = static_cast<int>(m_animationClock.getElapsedTime().asSeconds() * m_moveSpeed * m_animationSpeed) % 4;

		m_sprite.setTextureRect({ frame * 16, (m_characterNumber + textureDir) * 32, 16, 32 });
	}
	else // If not moving
	{
		m_sprite.setTextureRect({ 0, (m_characterNumber + textureDir) * 32, 16, 32 });
	}
}

void Player::loadTextures()
{
	if (!m_spriteSheet.loadFromFile("ASSETS/IMAGES/character.png"))
	{
		throw("Error loading spritesheet");
	}

	m_sprite.setTexture(m_spriteSheet);

	m_sprite.setTextureRect({ 0, m_characterNumber * 32, 16, 32 });
	m_sprite.setOrigin(8.0f, 24.0f);
}
