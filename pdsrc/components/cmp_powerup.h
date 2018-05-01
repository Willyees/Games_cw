#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class PowerUpComponent : public Component {
private:
	//player texures
	std::vector<std::vector<sf::Texture>> player_textures;
	bool countdown = false;
	float jumptime_count;
protected:
	float _jumptime;
	std::weak_ptr<Entity> _player;

public:
	void addTextures(std::vector<sf::Texture>);
	void update(double dt) override;
	void render() override {}
	explicit PowerUpComponent(Entity* p, float jumptime = 4.f);
	PowerUpComponent() = delete;
};
