#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class EnemyTurretComponent : public Component {
protected:
  void fire() const;
  float _firetime;
  float _speed;
  std::weak_ptr<Entity> _player;

public:
  void update(double dt) override;
  void render() override {}
  void setSpeed(float& speed);
  explicit EnemyTurretComponent(Entity* p);
  EnemyTurretComponent() = delete;
};
