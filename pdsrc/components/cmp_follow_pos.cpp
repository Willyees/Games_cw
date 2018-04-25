#include "cmp_follow_pos.h"
#include "cmp_sprite.h"
#include <iostream>

using namespace sf;
using namespace std;

void FollowPosComponent::update(double dt)
{
	_parent->setPosition(Vector2f(_follow->getPosition().x + _follow->GetCompatibleComponent<SpriteComponentAnimated>()[0]->getSprite().getLocalBounds().width / 2, _follow->getPosition().y + _follow->GetCompatibleComponent<SpriteComponentAnimated>()[0]->getSprite().getLocalBounds().height));
	
}

FollowPosComponent::FollowPosComponent(Entity * p, Entity* follow)
	: Component(p), _follow(follow)
{
}
