#pragma once
#include"SFML\System\Vector2.hpp"
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
	sf::Vector2f video_resolution;
	unsigned int fullscreen;
	KeyboardPreferences keyboard;
	ControllerPreferences controller;

	UserPreferences();
};


