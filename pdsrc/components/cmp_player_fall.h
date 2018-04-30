
#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class FallComponent : public Component {

protected:

	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit FallComponent(Entity* p);
	FallComponent() = delete;
};