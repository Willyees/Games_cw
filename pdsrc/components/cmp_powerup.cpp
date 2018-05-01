#include "cmp_powerup.h"
#include <engine.h>
#include "cmp_player_physics.h"
using namespace std;
using namespace sf;

void PowerUpComponent::addTextures(std::vector<Texture> textures)
{
	player_textures.push_back(textures);
}

void PowerUpComponent::update(double dt) {
	
	if (auto pl = _player.lock()) {
		if (countdown) {
			jumptime_count -= dt;
			
			if (jumptime_count <= 0.f) {
				//woudl be better to preload it during loading screen and then assign it when needed
				pl->get_components<PlayerPhysicsComponent>()[0]->setImpStrenght(-10.f);
				pl->get_components<SpriteComponentAnimated>()[0]->swapTextures(player_textures.at(0));
				_parent->setForDelete();

			}
			return;
		}
		if (length(pl->getPosition() - _parent->getPosition()) < 50.0) {
			_parent->setVisible(false);
			countdown = true;			
			pl->get_components<PlayerPhysicsComponent>()[0]->setImpStrenght(-12.f);
			
			pl->get_components<SpriteComponentAnimated>()[0]->swapTextures(player_textures.at(1));
			
		}
		
	}
}

PowerUpComponent::PowerUpComponent(Entity* p, float jumptime)
	: Component(p), _player(_parent->scene->ents.find("player")[0]), _jumptime(jumptime), jumptime_count(jumptime) {}
