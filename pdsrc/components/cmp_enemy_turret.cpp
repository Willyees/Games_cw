#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <cmath>
#include <iostream>
using namespace std;
using namespace sf;

void EnemyTurretComponent::update(double dt) {
  _firetime -= dt;
  if (_firetime <= 0.f) {
    fire();
    _firetime = 1.f;
  }
  static float angle = 0.f;
  angle += 1.f * dt;

  _parent->setRotation(180.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() const {
	auto bullet = _parent->scene->makeEntity(true);
	bullet->entityType = EntityType::BULLET;
	bullet->setPosition(_parent->getPosition());
	bullet->addComponent<HurtComponent>();
	bullet->addComponent<BulletComponent>();
	
	auto s = bullet->addComponent<SpriteComponentAnimated>();
	Texture p1;
	p1.loadFromFile("res/images/chef_knife50.png");
	Animation a;
	s->addFrames(a, 4, 1, 50.0f, 44.75f,0.0f);

	AnimatedSprite as(sf::seconds(0.05f), true, true);
	a.setSpriteSheet(*(s->addTexture(p1)));
	as.setOrigin(25.f, 22.4f);
	s->addSprite("idle", as, a);

	
	/*auto s = bullet->addComponent<ShapeComponent>();
	s->setShape<sf::CircleShape>(8.f);
	s->getShape().setFillColor(Color::Red);
	*/
	auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(50.f, 44.75f));
	p->setRestitution(.1f);
	p->setFriction(.005f);
	if (auto pl = _player.lock()) {
		//p->impulse(Vector2f(-50.0f, 25.0f));
		/*p->impulse(sf::rotate(Vector2f(0, 15.f), -_parent->getRotation()));*/
		//improve using center of entity
		Vector2f direction(pl->getPosition().x - _parent->getPosition().x, pl->getPosition().y - _parent->getPosition().y);
		float length_vector = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		p->impulse(direction / length_vector * _speed);
		
	}
}

void EnemyTurretComponent::setSpeed(float& speed) { _speed = speed; }

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) , _player(_parent->scene->ents.find("player")[0]), _speed(20.0f){}
