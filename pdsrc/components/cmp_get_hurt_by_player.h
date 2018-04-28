#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class GetHurtByPlayerComponent : public Component {
protected:
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit GetHurtByPlayerComponent(Entity* p);
	GetHurtByPlayerComponent() = delete;
};