#include "cmp_hurt_player.h"
#include"cmp_life.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
      /*pl->get_components<LifeComponent>()[0]->reduceLives();
      _parent->setForDelete();*/
    }
  }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}
