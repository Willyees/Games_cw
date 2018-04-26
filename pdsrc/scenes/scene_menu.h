#pragma once

#include <SFML\Audio\Music.hpp>
#include "engine.h"

class MenuScene : public Scene {
private:
	std::vector<sf::Vector2f> posMenuItems;
public:
	sf::Music theme;

  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};
