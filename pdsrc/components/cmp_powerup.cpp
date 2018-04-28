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
				_parent->setForDelete();
			}
			return;
		}
		if (length(pl->getPosition() - _parent->getPosition()) < 50.0) {
			_parent->setVisible(false);
			countdown = true;			
			pl->get_components<PlayerPhysicsComponent>()[0]->setImpStrenght(-20.f);
						
		}
		
	}
}

PowerUpComponent::PowerUpComponent(Entity* p, float jumptime)
	: Component(p), _player(_parent->scene->ents.find("player")[0]), _jumptime(jumptime) {}
