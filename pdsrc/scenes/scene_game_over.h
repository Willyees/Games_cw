#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>

class GameOverScene : public Scene {
private:
	sf::Music theme;
public:
	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	
};
