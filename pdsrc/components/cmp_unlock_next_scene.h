#pragma once
#include <ecm.h>

class UnlockNextSceneComponent : public Component {
public:
	
	void update(double dt) override;
	void render() override {}
	explicit UnlockNextSceneComponent(Entity* p);
	UnlockNextSceneComponent() = delete;
};
