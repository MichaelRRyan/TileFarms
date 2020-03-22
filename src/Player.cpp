#include "Player.h"

Player::Player(World& t_world) :
	m_world{ t_world },
	m_height{ 1 },
	m_velocity{ 0.0f, 0.0f },
	m_characterNumber{ 0 },
	m_animationSpeed{ 10.0f },
	m_DEFAULT_MOVE_SPEED{ 0.5f },
	m_moveSpeed{ m_DEFAULT_MOVE_SPEED },
	m_state{ State::Walking },
	m_direction{ 0, 1 } 
{
	loadTextures();

	if (m_controller.connect())
	{
		m_controller.setJoystickDeadzone(30.0f);
	}
}

///////////////////////////////////////////////////////////////////
void Player::update()
{
	if (m_controller.isConnected())
	{
		m_controller.update();
	}

	if (State::Walking == m_state)
	{
		if (m_height > 0 && m_world.getTileType(getX(), getY(), m_height - 1) == TileType::Null)
		{
			m_height--;
			std::cout << "Fell. Level: " << m_height << std::endl;
		}

		handleInput();
	}

	animate();
}

///////////////////////////////////////////////////////////////////
void Player::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_sprite);
}

///////////////////////////////////////////////////////////////////
unsigned const Player::getHeight() const
{
	return m_height;
}

///////////////////////////////////////////////////////////////////
unsigned const Player::getY() const
{
	return m_sprite.getPosition().y / Globals::TILE_SIZE;
}

///////////////////////////////////////////////////////////////////
unsigned const Player::getX() const
{
	return m_sprite.getPosition().x / Globals::TILE_SIZE;
}

///////////////////////////////////////////////////////////////////
sf::Vector2f const& Player::getPixelPosition() const
{
	return m_sprite.getPosition();
}

///////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////
void Player::handleInput()
{
	if (handleClimbEvent())
	{
		return;
	}

	handleDestroyEvent();
	handleBuildEvent();

	// Basic movement
	m_moveSpeed = m_DEFAULT_MOVE_SPEED;

	std::cout << vmath::length(m_controller.getCurrentState().LeftThumbStick) << std::endl;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
		|| vmath::length(m_controller.getCurrentState().LeftThumbStick) > 80.0f
		|| m_controller.getCurrentState().LTrigger > 0.0f
		|| m_controller.getCurrentState().RTrigger > 0.0f)
	{
		m_moveSpeed = m_DEFAULT_MOVE_SPEED * 3.0f;
	}

	handleMovement(getInputVector());
}

///////////////////////////////////////////////////////////////////
void Player::handleMovement(sf::Vector2f const& t_inputVector)
{
	m_velocity = { 0.0f, 0.0f };

	if (vmath::length(t_inputVector) != 0.0f)
	{
		sf::Vector2f const movementVector = vmath::unitVector(t_inputVector) * m_moveSpeed;

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

///////////////////////////////////////////////////////////////////
sf::Vector2f const Player::getInputVector() const
{
	sf::Vector2f inputVector;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
		|| m_controller.getCurrentState().DpadLeft
		|| m_controller.getCurrentState().LeftThumbStick.x < 0.0f)
	{
		inputVector.x--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
		|| m_controller.getCurrentState().DpadRight
		|| m_controller.getCurrentState().LeftThumbStick.x > 0.0f)
	{
		inputVector.x++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
		|| m_controller.getCurrentState().DpadUp
		|| m_controller.getCurrentState().LeftThumbStick.y < 0.0f)
	{
		inputVector.y--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		|| m_controller.getCurrentState().DpadDown
		|| m_controller.getCurrentState().LeftThumbStick.y > 0.0f)
	{
		inputVector.y++;
	}

	return inputVector;
}

///////////////////////////////////////////////////////////////////
bool Player::handleClimbEvent()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
		|| m_controller.getCurrentState().FaceBottom)
	{
		// Find the tile index of the tile in front of the player
		sf::Vector2i nextTile{ static_cast<int>(getX()) + m_direction.x, static_cast<int>(getY()) + m_direction.y };

		if (nextTile.x >= 0 && nextTile.x < Globals::WORLD_WIDTH_X
			&& nextTile.y >= 0 && nextTile.y < Globals::WORLD_WIDTH_Y)
		{
			if (TileType::Slope == m_world.getTileType(nextTile.x, nextTile.y, m_height))
			{
				// Find the tile after the next tile
				nextTile += m_direction;

				if (m_height + 1 < Globals::WORLD_HEIGHT)
				{
					if (nextTile.x >= 0 && nextTile.x < Globals::WORLD_WIDTH_X
						&& nextTile.y >= 0 && nextTile.y < Globals::WORLD_WIDTH_Y)
					{
						if (TileType::Null == m_world.getTileType(nextTile.x, nextTile.y, m_height + 1)
							&& TileType::Grass == m_world.getTileType(nextTile.x, nextTile.y, m_height))
						{
							m_previousPosition = m_sprite.getPosition();
							m_targetPosition = static_cast<sf::Vector2f>(nextTile)* Globals::TILE_SIZE + (sf::Vector2f{ 0.5f, 0.5f } *Globals::TILE_SIZE);

							m_height++;

							m_state = State::Climbing;
							m_animationClock.restart();
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////
void Player::handleDestroyEvent()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)
		|| m_controller.getCurrentState().FaceRight)
	{
		// Find the tile index of the tile in front of the player
		sf::Vector2i nextTile{ static_cast<int>(getX()) + m_direction.x, static_cast<int>(getY()) + m_direction.y };

		if (nextTile.x >= 0 && nextTile.x < Globals::WORLD_WIDTH_X
			&& nextTile.y >= 0 && nextTile.y < Globals::WORLD_WIDTH_Y)
		{
			if (TileType::Null != m_world.getTileType(nextTile.x, nextTile.y, m_height))
			{
				if (m_height + 1 < Globals::WORLD_HEIGHT
					&& TileType::Null == m_world.getTileType(nextTile.x, nextTile.y, m_height + 1))
				{
					m_world.destroyTile(nextTile.x, nextTile.y, m_height);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
void Player::handleBuildEvent()
{
	// Check for key down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)
		|| m_controller.getCurrentState().FaceLeft)
	{
		// Find the tile index of the tile in front of the player
		sf::Vector2i nextTile{ static_cast<int>(getX()) + m_direction.x, static_cast<int>(getY()) + m_direction.y };

		// Check the tile is within world bounds
		if (nextTile.x >= 0 && nextTile.x < Globals::WORLD_WIDTH_X
			&& nextTile.y >= 0 && nextTile.y < Globals::WORLD_WIDTH_Y)
		{
			// Check if the tile is empty
			if (TileType::Null == m_world.getTileType(nextTile.x, nextTile.y, m_height))
			{
				// Check that there is a tile underneath or is on the bottom of the world
				if (m_height - 1 < 0 || TileType::Null != m_world.getTileType(nextTile.x, nextTile.y, m_height - 1))
				{
					m_world.buildTile(TileType::Grass, nextTile.x, nextTile.y, m_height);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
void Player::animate()
{
	int textureDir = m_sprite.getTextureRect().top / 32 - m_characterNumber;

	if (State::Walking == m_state)
	{
		// Check if moving
		if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
		{
			// Set the texture direction depending on the direction we're facing
			if (m_velocity.x > 0.0f)
			{
				textureDir = 1;
				m_sprite.setScale(1.0f, 1.0f);
				m_direction = { 1, 0 };
			}
			else if (m_velocity.x < 0.0f)
			{
				textureDir = 1;
				m_sprite.setScale(-1.0f, 1.0f);
				m_direction = { -1, 0 };
			}
			else if (m_velocity.y < 0.0f)
			{
				textureDir = 2;
				m_direction = { 0, -1 };
			}
			else if (m_velocity.y > 0.0f)
			{
				textureDir = 0;
				m_direction = { 0, 1 };
			}

			int frame = static_cast<int>(m_animationClock.getElapsedTime().asSeconds() * m_moveSpeed * m_animationSpeed) % 4;

			m_sprite.setTextureRect({ frame * 16, (m_characterNumber + textureDir) * 32, 16, 32 });
		}
		else // If not moving
		{
			m_sprite.setTextureRect({ 0, (m_characterNumber + textureDir) * 32, 16, 32 });
		}
	}
	else if (State::Climbing == m_state)
	{
		if (m_animationClock.getElapsedTime().asSeconds() < 1.0f)
		{
			sf::Vector2f newPosition = m_previousPosition + (m_targetPosition - m_previousPosition) * (m_animationClock.getElapsedTime().asSeconds() / 1.0f);

			m_sprite.setPosition(newPosition);

			int frame = 1 - static_cast<int>(m_animationClock.getElapsedTime().asSeconds() / 1.0f * 2.0f);

			m_sprite.setTextureRect({ 80 + frame * 16, (m_characterNumber + textureDir) * 32, 16, 32 });
		}
		else
		{
			m_sprite.setPosition(m_targetPosition);
			m_state = State::Walking;
		}
	}
}

///////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////