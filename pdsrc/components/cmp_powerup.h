#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PowerUpComponent : public Component {
private:
	bool countdown = false;
protected:
	float _jumptime;
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit PowerUpComponent(Entity* p, float jumptime = 4.f);
	PowerUpComponent() = delete;
};
