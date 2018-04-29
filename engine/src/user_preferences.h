#pragma once

#include "SFML\Window\WindowStyle.hpp"
#include "SFML\Window\Keyboard.hpp"
#include "SFML\Window\Joystick.hpp"

struct KeyboardPreferences
{
	sf::Keyboard::Key UP;
	sf::Keyboard::Key LEFT;
	sf::Keyboard::Key RIGHT;

	KeyboardPreferences();
};

struct ControllerPreferences
{
	unsigned int UP;

	ControllerPreferences();
};

struct UserPreferences
{
public:
	int video_resolution;
	KeyboardPreferences keyboard;
	ControllerPreferences controller;

	UserPreferences();
};


