#include "cmp_get_hurt_by_player.h"
#include <engine.h>

using namespace std;

void GetHurtByPlayerComponent::update(double dt)
{
	if (auto pl = _player.lock()) {
		
		if (length(pl->getPosition() - _parent->getPosition()) < 50.0) {
			_parent->setForDelete();
			_parent->scene->addScore(10);
			
			
		}
	}
}

GetHurtByPlayerComponent::GetHurtByPlayerComponent(Entity * p)
	: Component(p), _player(_parent->scene->ents.find("player")[0]) {}
