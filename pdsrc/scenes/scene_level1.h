#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>

class Level1Scene : public Scene {
	sf::Music theme;
public:

  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void collisionHandler(Entity* entityA, Entity* entityB) override;
};
