#include "user_preferences.h"
#include <iostream>

using namespace sf;
using namespace std;

UserPreferences::UserPreferences() :
	fullscreen(7), video_resolution(Vector2f(800,600)), changed_fullscreen(false), changed_resolution(false){
	
} //7 default; 8 fullscreen

KeyboardPreferences::KeyboardPreferences() : 
	UP(Keyboard::Key::Up), LEFT(Keyboard::Key::Left), RIGHT(Keyboard::Key::Right){
	
}

ControllerPreferences::ControllerPreferences() :
	UP(0){
}
