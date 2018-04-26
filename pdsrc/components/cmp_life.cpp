#include "cmp_life.h"
#include "cmp_blinking.h"
#include "engine.h"
#include "cmp_sprite.h"
#include <iostream>

void LifeComponent::update(double dt)
{
	static sf::Clock clock;
	if (_lives < 1)
		_parent->setForDelete();
	if (_damaged) {
		clock.restart();
		_damaged = false;
		Engine::getActiveScene()->ents.find("life")[0]->get_components<SpriteComponentRepeted>()[0]->decreaseRep();
	}
	if (_state == "invincible")
	{
		float dt = clock.restart().asSeconds();
		static float timer = 0.0f;
		timer += dt;
		
		if (timer > _invincibleTime){
			_state = "vulnerable";
			timer = 0.0f;
			_parent->get_components<BlinkComponent>()[0]->_fordeletion = true;
			_parent->setVisible(true);//make sure entity is visible
			
		}
	}
}

int LifeComponent::getLives()
{
	return _lives;
}

void LifeComponent::reduceLives() { 
	if(_state != "invincible"){
		_lives --;
		_state = "invincible";
		_damaged = true;
		_parent->addComponent<BlinkComponent>(0.3f);
		std::cout << _lives << std::endl;
	} 
	
		
	
}

void LifeComponent::increaseLives() { _lives++; }

LifeComponent::LifeComponent(Entity * p, int lives) 
	: Component(p), _lives(lives), _state("vulnerable"), _invincibleTime(4.0f), _damaged(false)
{
}
