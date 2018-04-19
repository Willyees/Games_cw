#pragma once
#include <ecm.h>

class LifeComponent : public Component {
private:
	int _lives;

public:
	void update(double dt) override;
	void render() override {};
	int getLives();
	void reduceLives();
	void increaseLives();
	explicit LifeComponent(Entity* p, int lives);
	LifeComponent() = delete;
};