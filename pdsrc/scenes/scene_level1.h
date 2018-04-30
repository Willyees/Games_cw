#pragma once

#include "engine.h"
#include <SFML\Audio\Music.hpp>
#include <SFML\Graphics\View.hpp>

class Level1Scene : public Scene {
private:
	sf::Music theme;
	bool _nextSceneUnlocked = false;


	// whether game is paused.
	bool _paused = false;

	// Pause/play button
	sf::Texture _pausetex;
	sf::Texture _playtex;
	sf::Sprite _pausesprite;
	sf::IntRect _pauserect;

	// Restart game button.
	sf::Texture _resettex;
	sf::Sprite _resetsprite;
	sf::IntRect _resetrect;

	// Back to menu button.
	sf::Texture _backtex;
	sf::Sprite _backsprite;
	sf::IntRect _backrect;

	// Savegame button.
	sf::Texture _savetex;
	sf::Sprite _savesprite;
	sf::IntRect _saverect;

	// Loadgame button.
	sf::Texture _loadtex;
	sf::Sprite _loadsprite;
	sf::IntRect _loadrect;

	void LoadState();
	void SaveState();

public:
  void UnlockNextScene();
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void collisionHandler(Entity* entityA, Entity* entityB) override;
};
