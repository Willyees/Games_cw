#pragma once

#include "engine.h"

class GameOverScene : public Scene {
public:
	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	
};
