#include "user_preferences.h"
#include <iostream>

using namespace sf;
using namespace std;

UserPreferences::UserPreferences() :
	video_resolution(7) {
	
} //7 default; 8 fullscreen

KeyboardPreferences::KeyboardPreferences() : 
	UP(Keyboard::Key::Up), LEFT(Keyboard::Key::Left), RIGHT(Keyboard::Key::Right){
	
}

ControllerPreferences::ControllerPreferences() :
	UP(0){
	
}
