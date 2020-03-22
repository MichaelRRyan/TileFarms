#include "Gamepad.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 22/03/2020
/// </summary>

int Gamepad::s_noOfControllers{ 0 };

///////////////////////////////////////////////////////////////////
Gamepad::Gamepad() :
	m_joystickIndex{ 0 }, // Set a default index of 0 until connected
	m_controllerType{ ControllerType::XBox360 }
{
}

///////////////////////////////////////////////////////////////////
void Gamepad::update()
{
	// Set the previous state
	m_previousState = m_currentState;

	// Update based on controller type
	switch (m_controllerType)
	{
	case ControllerType::XBox360:
		updateXBox360();
		break;
	case ControllerType::Playstation4:
		updatePlaystation4();
		break;
	}

	confineAnalogSticksToDeadzone();
}

///////////////////////////////////////////////////////////////////
bool Gamepad::connect()
{
	// Loop through all supported joysticks
	for (int i = s_noOfControllers; i < sf::Joystick::Count; i++)
	{
		if (sf::Joystick::isConnected(i))
		{
			// Set the controller indices
			m_joystickIndex = i;
			s_noOfControllers++;

			// Try detect the controller type
			sf::Joystick::Identification identity{ sf::Joystick::getIdentification(m_joystickIndex) };

			if (identity.name.find("Xbox") != sf::String::InvalidPos
				|| identity.name.find("360") != sf::String::InvalidPos)
			{
				m_controllerType = ControllerType::XBox360;

#ifdef _DEBUG
				std::cout << "Xbox 360 controller detected" << std::endl;
#endif // _DEBUG
			}
			else if (identity.name.find("Playstation") != sf::String::InvalidPos
				|| identity.name.find("PS") != sf::String::InvalidPos
				|| identity.name.find("Wireless Controller") != sf::String::InvalidPos) // Default PS4 Wireless Controller name
			{
				m_controllerType = ControllerType::Playstation4;

#ifdef _DEBUG
				std::cout << "Playstation 4 controller detected" << std::endl;
#endif // _DEBUG
			}
			else // Default to XBox 360 if couldn't detect a type
			{
				m_controllerType = ControllerType::XBox360;

#ifdef _DEBUG
				std::cout << "No type could be detected. Defaulting to XBox360" << std::endl;
#endif // _DEBUG
			}

#ifdef _DEBUG
			std::cout << "Connected controller ID: " << i << std::endl;
			std::cout << static_cast<std::string>(identity.name) << std::endl;
			std::cout << "Vendor ID: " << identity.vendorId << std::endl;
			std::cout << "Product ID: " << identity.productId << std::endl;
#endif // _DEBUG

			return true;
		}
	}

	return false; // Return if no available controllers to connect to
}

///////////////////////////////////////////////////////////////////
bool const Gamepad::isConnected() const
{
	return sf::Joystick::isConnected(m_joystickIndex);
}

///////////////////////////////////////////////////////////////////
GamepadState const& Gamepad::getCurrentState() const
{
	return m_currentState;
}

///////////////////////////////////////////////////////////////////
GamepadState const& Gamepad::getPreviousState() const
{
	return m_previousState;
}

///////////////////////////////////////////////////////////////////
int const Gamepad::getNoOfConnectedControllers() const
{
	return s_noOfControllers;
}

///////////////////////////////////////////////////////////////////
void Gamepad::setControllerType(ControllerType const t_type)
{
	m_controllerType = t_type;
}

///////////////////////////////////////////////////////////////////
ControllerType const Gamepad::getControllerType() const
{
	return m_controllerType;
}

///////////////////////////////////////////////////////////////////
void Gamepad::setJoystickDeadzone(float const t_deadzone)
{
	m_joystickDeadzone = t_deadzone;
}

///////////////////////////////////////////////////////////////////
float const Gamepad::getJoystickDeadzone() const
{
	return m_joystickDeadzone;
}

///////////////////////////////////////////////////////////////////
void Gamepad::updateXBox360()
{
	// Face buttons
	m_currentState.FaceBottom = sf::Joystick::isButtonPressed(m_joystickIndex, 0); // A
	m_currentState.FaceRight = sf::Joystick::isButtonPressed(m_joystickIndex, 1); // B
	m_currentState.FaceLeft = sf::Joystick::isButtonPressed(m_joystickIndex, 2); // X
	m_currentState.FaceTop = sf::Joystick::isButtonPressed(m_joystickIndex, 3); // Y

	// Bumper buttons
	m_currentState.LB = sf::Joystick::isButtonPressed(m_joystickIndex, 4); // LB
	m_currentState.RB = sf::Joystick::isButtonPressed(m_joystickIndex, 5); // RB

	// Option buttons
	m_currentState.Back = sf::Joystick::isButtonPressed(m_joystickIndex, 6); // Back
	m_currentState.Start = sf::Joystick::isButtonPressed(m_joystickIndex, 7); // Start

	m_currentState.Share = sf::Joystick::isButtonPressed(m_joystickIndex, 6); // Equivalent: Back
	m_currentState.Options = sf::Joystick::isButtonPressed(m_joystickIndex, 7); // Equivalent: Start

	// Thumb stick click
	m_currentState.LeftThumbStickClick = sf::Joystick::isButtonPressed(m_joystickIndex, 8); // Left thumbstick click
	m_currentState.RightThumbStickClick = sf::Joystick::isButtonPressed(m_joystickIndex, 9); // Right thumbstick click

	// Trigger buttons
	if (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z) > m_XBOX_TRIGGER_DEADZONE)
	{
		m_currentState.LTrigger = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z); // Left trigger
		m_currentState.RTrigger = 0.0f;
	}
	else if (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z) < -m_XBOX_TRIGGER_DEADZONE)
	{
		m_currentState.RTrigger = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z) * -1; // Right trigger
		m_currentState.LTrigger = 0.0f;
	}
	else
	{
		m_currentState.RTrigger = 0.0f;
		m_currentState.LTrigger = 0.0f;
	}

	// Dpad
	m_currentState.DpadRight = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX) > m_DPAD_DEADZONE); // Dpad right
	m_currentState.DpadLeft = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX) < -m_DPAD_DEADZONE); // Dpad left
	m_currentState.DpadUp = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY) > m_DPAD_DEADZONE); // Dpad up
	m_currentState.DpadDown = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY) < -m_DPAD_DEADZONE); // Dpad down

	// Analog sticks
	m_currentState.LeftThumbStick.x = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::X); // Left thumb stick X-Axis
	m_currentState.LeftThumbStick.y = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Y); // Left thumb stick Y-Axis

	m_currentState.RightThumbStick.x = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::U); // Right thumb stick X-Axis
	m_currentState.RightThumbStick.y = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::V); // Right thumb stick Y-Axis
}

///////////////////////////////////////////////////////////////////
void Gamepad::updatePlaystation4()
{
	// Face buttons
	m_currentState.FaceLeft = sf::Joystick::isButtonPressed(m_joystickIndex, 0); // Square button
	m_currentState.FaceBottom = sf::Joystick::isButtonPressed(m_joystickIndex, 1); // X button
	m_currentState.FaceRight = sf::Joystick::isButtonPressed(m_joystickIndex, 2); // Circle button
	m_currentState.FaceTop = sf::Joystick::isButtonPressed(m_joystickIndex, 3); // Triangle

	// Bumper buttons
	m_currentState.LB = sf::Joystick::isButtonPressed(m_joystickIndex, 4); // LB
	m_currentState.RB = sf::Joystick::isButtonPressed(m_joystickIndex, 5); // RB

	// Options buttons
	m_currentState.Share = sf::Joystick::isButtonPressed(m_joystickIndex, 8);
	m_currentState.Options = sf::Joystick::isButtonPressed(m_joystickIndex, 9);

	m_currentState.Back = sf::Joystick::isButtonPressed(m_joystickIndex, 8); // Equivalent: Share
	m_currentState.Start = sf::Joystick::isButtonPressed(m_joystickIndex, 9); // Equivalent: Options

	// Thumb stick click
	m_currentState.LeftThumbStickClick = sf::Joystick::isButtonPressed(m_joystickIndex, 10); // Left thumbstick click

	m_currentState.RightThumbStickClick = sf::Joystick::isButtonPressed(m_joystickIndex, 11); // Right thumbstick click

	// Trigger buttons
	// Triggers return a value between -100 (not pressed) and 100 (pressed fully)
	// By adding 100, dividing by max value (200) we get a value between 0 (not pressed) and 1 (fully pressed)
	// Multiplying by 100 gets a value between 0 and 100 for easier use
	m_currentState.LTrigger = ((sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::U) + 100.0f) / 200.0f) * 100.0f; // Left trigger
	m_currentState.RTrigger = ((sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::V) + 100.0f) / 200.0f) * 100.0f; // Right trigger

	// Dpad
	m_currentState.DpadLeft = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX) < -m_DPAD_DEADZONE); // Dpad left
	m_currentState.DpadRight = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX) > m_DPAD_DEADZONE); // Dpad right
	m_currentState.DpadUp = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY) > m_DPAD_DEADZONE); // Dpad up
	m_currentState.DpadDown = (sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY) < -m_DPAD_DEADZONE); // Dpad down

	// Analog sticks
	m_currentState.LeftThumbStick.x = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::X); // Left thumb stick X-Axis
	m_currentState.LeftThumbStick.y = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Y); // Left thumb stick Y-Axis

	m_currentState.RightThumbStick.x = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z); // Right thumb stick X-Axis
	m_currentState.RightThumbStick.y = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::R); // Right thumb stick Y-Axis
}

///////////////////////////////////////////////////////////////////
void Gamepad::confineAnalogSticksToDeadzone()
{
	// Left stick
	if (m_currentState.LeftThumbStick.x > -m_joystickDeadzone && m_currentState.LeftThumbStick.x < m_joystickDeadzone)
	{
		m_currentState.LeftThumbStick.x = 0.0f;
	}

	if (m_currentState.LeftThumbStick.y > -m_joystickDeadzone && m_currentState.LeftThumbStick.y < m_joystickDeadzone)
	{
		m_currentState.LeftThumbStick.y = 0.0f;
	}

	// Right stick
	if (m_currentState.RightThumbStick.x > -m_joystickDeadzone && m_currentState.RightThumbStick.x < m_joystickDeadzone)
	{
		m_currentState.RightThumbStick.x = 0.0f;
	}

	if (m_currentState.RightThumbStick.y > -m_joystickDeadzone && m_currentState.RightThumbStick.y < m_joystickDeadzone)
	{
		m_currentState.RightThumbStick.y = 0.0f;
	}
}

///////////////////////////////////////////////////////////////////