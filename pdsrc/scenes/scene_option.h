#pragma once

#include "engine.h"

class OptionScene : public Scene {
public:

	OptionScene() = default;
	~OptionScene() override = default;

	void Load() override;
	void Update(const double& dt) override;
};