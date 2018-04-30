#include "cmp_change_state_close_player.h"
#include "engine.h"

void ChangeStateClosePlayerComponent::update(double dt)
{
	if (auto pl = _player.lock()) {

		if (length(pl->getPosition() - _parent->getPosition()) < 600.0) {
			_parent->setState("idle");

		}
		else
			_parent->setState("none");
	}
}

ChangeStateClosePlayerComponent::ChangeStateClosePlayerComponent(Entity * p) 
	: Component(p), _player(_parent->scene->ents.find("player")[0])
{
}
