#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>

class Level1Scene : public Scene {
private:
	sf::Music theme;
	bool _nextSceneUnlocked = false;
public:
  void UnlockNextScene();
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void collisionHandler(Entity* entityA, Entity* entityB) override;
};
