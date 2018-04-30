#pragma once

#include "engine.h"
#include "SFML\Audio.hpp>

class Level1Scene : public Scene {
	sf::SoundBuffer themebuff;
	sf::Sound theme;

public:

  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void collisionHandler(Entity* entityA, Entity* entityB) override;
};
