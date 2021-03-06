#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/window/Joystick.hpp>

using namespace std;
using namespace sf;
using namespace Physics;

bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();
  static string previous;
  //Teleport to start if we fall off map.
  if (pos.y > ls::getHeight() * ls::getTileSize()) {
    teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
  }

  if (Keyboard::isKeyPressed(Engine::user_preferences.keyboard.LEFT) ||
      Keyboard::isKeyPressed(Engine::user_preferences.keyboard.RIGHT) ||
	  Joystick::getAxisPosition(0, Joystick::X) < -10.0f || Joystick::getAxisPosition(0, Joystick::X) > 10.0f) {
    // Moving Either Left or Right
    if (Keyboard::isKeyPressed(Engine::user_preferences.keyboard.RIGHT) ||
		Joystick::getAxisPosition(0, Joystick::X) > 10.0f) {
		
      if (getVelocity().x < _maxVelocity.x && (pos.x + dt * _groundspeed) < ls::getWidth())
        impulse({(float)(dt * _groundspeed), 0});

		  if (!isGrounded())
			  if (previous == "in air right" || previous == "in air left")
				  _parent->setState(previous);
			  else
				  _parent->setState("none");
		  else {
			 _parent->setState("right");
			 previous = "right";
		  }
    } else {
	  if ((getVelocity().x > -_maxVelocity.x) && ((pos.x - dt * _groundspeed) > 50.0f))//assuming map starts at pos 0
        impulse({-(float)(dt * _groundspeed), 0});
	  if (!isGrounded())
		  if (previous == "in air right" || previous == "in air left")
			  _parent->setState(previous);
		  else
			  _parent->setState("none");
	  else{
		_parent->setState("left");
		previous = "left";
	  }
	}
  } else {
	  _parent->setState("none");
    // Dampen X axis movement
    dampen({0.9f, 1.0f});
  }
  
  // Handle Jump
  if (Keyboard::isKeyPressed(Engine::user_preferences.keyboard.UP) || 
	  Joystick::isButtonPressed(0, Engine::user_preferences.controller.UP)) {
    _grounded = isGrounded();
	if (previous == "right") {
		_parent->setState("in air right");
		previous = "in air right";
	}
	if (previous == "left") {
		_parent->setState("in air left");
		previous = "in air left";
	}
    if (_grounded) {
	  setVelocity(Vector2f(getVelocity().x, 0.f));
      teleport(Vector2f(pos.x, pos.y - 2.0f));
      impulse(Vector2f(0, _impstreght));
	  /*impstreght(Vector2f(0, -11.f));*/
    }
  }

  //Are we in air?
  if (!_grounded) {
	  _parent->setState(previous);
    // Check to see if we have landed yet
    _grounded = isGrounded();
    // disable friction while jumping
    setFriction(0.f);
  } else {
    setFriction(0.1f);
  }
  
  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}


PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
                                               const Vector2f& size)
    : PhysicsComponent(p, true, size, true) {
  _impstreght = -10.f;
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 400.f);
  _groundspeed = 30.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
}

void PlayerPhysicsComponent::setImpStrenght(float impstreght)
{
	_impstreght = impstreght;
}
