#pragma once

#include <SFML\Audio\Music.hpp>
#include "engine.h"

class MenuScene : public Scene {
private:
	sf::Vector2f posMenuItems[3];
public:
	sf::Music theme;

  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;
};
