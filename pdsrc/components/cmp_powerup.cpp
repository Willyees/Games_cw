#include "cmp_powerup.h"
#include <engine.h>
#include "cmp_player_physics.h"
using namespace std;
using namespace sf;

void PowerUpComponent::update(double dt) {
	
	static float jumptime = _jumptime;
	
	if (auto pl = _player.lock()) {
		if (countdown) {
			jumptime -= dt;
			cout << jumptime << endl;
			if (jumptime <= 0.f) {

				pl->get_components<PlayerPhysicsComponent>()[0]->setImpStrenght(-10.f);
				Texture p;
				p.loadFromFile("res/images/player_sprites/walk_right.png");
				temp.push_back(p);
				p.loadFromFile("res/images/player_sprites/walk_right.png");
				temp.push_back(p);
				p.loadFromFile("res/images/player_sprites/walk_left_1.png");
				temp.push_back(p);
				p.loadFromFile("res/images/player_sprites/jumpL.png");
				temp.push_back(p);
				p.loadFromFile("res/images/player_sprites/jumpR.png");
				temp.push_back(p);
				pl->get_components<SpriteComponentAnimated>()[0]->swapTextures(temp);
				_parent->setForDelete();

			}
			return;
		}
		if (length(pl->getPosition() - _parent->getPosition()) < 50.0) {
			_parent->setVisible(false);
			countdown = true;			
			pl->get_components<PlayerPhysicsComponent>()[0]->setImpStrenght(-20.f);
			vector<Texture> temp;
			Texture p;
			p.loadFromFile("res/images/player_sprites/walk_right.png");
			temp.push_back(p);
			p.loadFromFile("res/images/player_sprites/walk_right.png");
			temp.push_back(p);
			p.loadFromFile("res/images/player_sprites/walk_left_1_green.png");
			temp.push_back(p);
			p.loadFromFile("res/images/player_sprites/jumpL.png");
			temp.push_back(p);
			p.loadFromFile("res/images/player_sprites/jumpR.png");
			temp.push_back(p);
			pl->get_components<SpriteComponentAnimated>()[0]->swapTextures(temp);
			
		}
		
	}
}

PowerUpComponent::PowerUpComponent(Entity* p, float jumptime)
	: Component(p), _player(_parent->scene->ents.find("player")[0]), _jumptime(jumptime) {}
