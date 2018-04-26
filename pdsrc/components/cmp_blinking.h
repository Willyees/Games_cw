#pragma once
#include "ecm.h"
#include "cmp_life.h"

class BlinkComponent : public Component {
	friend class LifeComponent;
private:
	float blinktimer;
public:
	BlinkComponent() = delete;
	
	explicit BlinkComponent(Entity* p, float blinktimer);
	void update(double dt) override;
	void render() override;
};