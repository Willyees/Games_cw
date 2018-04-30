#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>
#include <SFML\Graphics\View.hpp>

class Level1Scene : public Scene {
private:
	sf::Music theme;
	bool _nextSceneUnlocked = false;

	bool _paused = false;
	sf::Texture _pausetex;
	sf::Texture _playtex;
	sf::Sprite _pausesprite;
	sf::IntRect _pauserect;

	sf::Texture _resettex;
	sf::Sprite _resetsprite;
	sf::IntRect _resetrect;

public:
  void UnlockNextScene();
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void collisionHandler(Entity* entityA, Entity* entityB) override;
};
