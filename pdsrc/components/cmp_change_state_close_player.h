#pragma once
#include <ecm.h>


class ChangeStateClosePlayerComponent : public Component {
protected:
	std::weak_ptr<Entity> _player;

public:
	void update(double dt) override;
	void render() override {}
	explicit ChangeStateClosePlayerComponent(Entity* p);
	ChangeStateClosePlayerComponent() = delete;
};