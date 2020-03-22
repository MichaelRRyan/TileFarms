#ifndef GAMEPAD_H
#define GAMEPAD_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 21/03/2020
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

struct GamepadState
{
	bool FaceBottom{ false }; // XBox360: A		PS4: X
	bool FaceRight{ false }; // XBox360: B		PS4: Circle
	bool FaceLeft{ false }; // XBox360: X		PS4: Square
	bool FaceTop{ false }; // XBox360: Y		PS4: Triangle
	bool LB{ false };
	bool RB{ false };
	bool LeftThumbStickClick{ false };
	bool RightThumbStickClick{ false };
	bool DpadUp{ false };
	bool DpadDown{ false };
	bool DpadLeft{ false };
	bool DpadRight{ false };
	bool Start{ false }; // Also Options button on PS4 controller
	bool Back{ false }; // Also Share button on PS4 controller
	bool Options{ false }; // Also Start button on XBox360 controller
	bool Share{ false }; // Also Back button on XBox360 controller
	float RTrigger{ 0.0f };
	float LTrigger{ 0.0f };
	sf::Vector2f LeftThumbStick{ 0.0f, 0.0f };
	sf::Vector2f RightThumbStick{ 0.0f, 0.0f };
};

enum class ControllerType
{
	XBox360,
	Playstation4
};

class Gamepad
{
public:

	Gamepad();

	void update();

	bool connect();

	bool const isConnected() const;

	GamepadState const & getCurrentState() const;

	GamepadState const& getPreviousState() const;

	int const getNoOfConnectedControllers() const;

	void setControllerType(ControllerType const t_type);

	ControllerType const getControllerType() const;

	void setJoystickDeadzone(float const t_deadzone);

	float const getJoystickDeadzone() const;

private:

	void updateXBox360();
	
	void updatePlaystation4();

	void confineAnalogSticksToDeadzone();

	// Number of connected controllers
	static int s_noOfControllers;

	// Deadzone for the dpad (really a joystick)
	const float m_DPAD_DEADZONE{ 50.0f };

	// Deadzone for the XBox360 triggers
	const float m_XBOX_TRIGGER_DEADZONE{ 5.0f };

	float m_joystickDeadzone{ 15.0f };

	// Index of the controller the object refers to
	int m_joystickIndex;

	// The current state of all the buttons
	GamepadState m_currentState;

	// The previous state to use to check for the moment a button is pressed
	GamepadState m_previousState;

	// The type of the controller the object refers to
	ControllerType m_controllerType;

};

#endif // !GAMEPAD_H
