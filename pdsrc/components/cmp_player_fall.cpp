#include "cmp_player_fall.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/window/Joystick.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

void FallComponent::update(double dt) {

	const auto pos = _parent->getPosition();
	static string previous;
	//Teleport to start if we fall off map.
	if (ls::getTilePosition(ls::findTiles(ls::INVISWALL)) {
		teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}
}