#pragma once
#include "ecm.h"

class FollowPosComponent : public Component {
protected:
	Entity * _follow;

public:
	void update(double dt) override;
	void render() override {}
	
	explicit FollowPosComponent(Entity* p, Entity* follow);
	FollowPosComponent() = delete;
};