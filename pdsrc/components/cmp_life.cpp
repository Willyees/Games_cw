#include "cmp_life.h"
#include <iostream>

void LifeComponent::update(double dt)
{
	if (_lives < 1)
		_parent->setForDelete();
}

int LifeComponent::getLives()
{
	return _lives;
}

void LifeComponent::reduceLives() { _lives --; 
std::cout << _lives << std::endl;
}

void LifeComponent::increaseLives() { _lives++; }

LifeComponent::LifeComponent(Entity * p, int lives) : Component(p), _lives(lives)
{
}
