#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;
  float _impstreght;

  bool isGrounded() const;

public:
  void update(double dt) override;
  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);
  void setImpStrenght(float impstreght);
  PlayerPhysicsComponent() = delete;
};
