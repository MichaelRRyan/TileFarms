#include "Player.h"

Player::Player(World& t_world) :
	m_world{ t_world },
	m_moveSpeed{ 0.5f },
	m_height{ 1 },
	m_velocity{ 0.0f, 0.0f },
	m_characterNumber{ 3 },
	m_animationSpeed{ 10.0f }
{
	loadTextures();
}

void Player::update()
{
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

unsigned const Player::getRow() const
{
	return m_sprite.getPosition().y / Globals::TILE_SIZE;
}

void Player::handleMovement()
{
	sf::Vector2f movementVector;
	m_velocity = { 0.0f, 0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movementVector.x--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movementVector.x++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movementVector.y--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movementVector.y++;
	}

	float const magnitude = sqrtf(movementVector.x * movementVector.x + movementVector.y * movementVector.y);

	if (0 != magnitude)
	{
		sf::Vector2f const moveDirection = movementVector / magnitude;
		sf::Vector2f const movement = moveDirection * m_moveSpeed;

		// Horisontal collisions
		int targetTileX = (m_sprite.getPosition().x + movement.x) / Globals::TILE_SIZE;
		
		if (targetTileX >= 0 && targetTileX < Globals::WORLD_WIDTH_X)
		{
			if (m_world.getTileType(targetTileX, m_sprite.getPosition().y / Globals::TILE_SIZE, m_height) != TileType::Rock)
			{
				m_velocity.x = movement.x;
			}
		}

		// Vertical collisions
		int targetTileY = (m_sprite.getPosition().y + movement.y) / Globals::TILE_SIZE;

		if (targetTileY >= 0 && targetTileY < Globals::WORLD_WIDTH_Y)
		{
			if (m_world.getTileType(m_sprite.getPosition().x / Globals::TILE_SIZE, targetTileY, m_height) != TileType::Rock)
			{
				m_velocity.y = movement.y;
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

		int frame = static_cast<int>(m_animationClock.getElapsedTime().asSeconds() * m_moveSpeed * m_animationSpeed) % 3;

		m_sprite.setTextureRect({ frame * 16, (m_characterNumber + textureDir) * 32, 16, 32 });
	}
	else // If not moving
	{
		m_sprite.setTextureRect({ 16, (m_characterNumber + textureDir) * 32, 16, 32 });
	}
}

void Player::loadTextures()
{
	if (!m_spriteSheet.loadFromFile("ASSETS/IMAGES/character_sheet.png"))
	{
		throw("Error loading spritesheet");
	}

	m_sprite.setTexture(m_spriteSheet);

	m_sprite.setTextureRect({ 16, m_characterNumber * 32, 16, 32 });
	m_sprite.setOrigin(Globals::TILE_SIZE / 2.0f, Globals::TILE_SIZE * 2.0);
	m_sprite.setPosition(Globals::TILE_SIZE / 2.0f, Globals::TILE_SIZE * 2.0f);
}
