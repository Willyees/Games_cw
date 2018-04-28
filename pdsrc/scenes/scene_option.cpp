#include "scene_option.h"
#include "../components/cmp_text.h"
#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_life.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_follow_pos.h"
#include "../components/cmp_hurt_player.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include <SFML\Graphics\View.hpp>
#include <SFML\Audio\Music.hpp>
#include "system_renderer.h"
#include "system_physics.h"

using namespace std;
using namespace sf;

void OptionScene::Load()
{
	
		Texture p;
		p.loadFromFile("res/images/31394910_1642512485826072_6333550360287772672_n.png");

		for (int i = 0; i < 6; i++) {

			for(int j =0; j <3; j++){
			
			shared_ptr<Entity> f_temp = makeEntity(true);
			f_temp->setPosition(Vector2f(50* j, 50*i));
			auto s = f_temp->addComponent<SpriteComponent>();
			s->setSprite(Sprite(*(s->setTexture(p)), IntRect(50 * j, 50 * i, 50,50)));//adding texture internally and giving it to the animation as well
			cout << 50 * j << "     " << 50 * i << endl;
			f_temp->entityType = EntityType::POWERUP;
			auto phy = f_temp->addComponent<PhysicsComponent>(false, Vector2f(40.0f, 40.0f));
			phy->setRestitution(0.0f);
		}
		}

		shared_ptr<Entity> f_temp = makeEntity(true);
		f_temp->setPosition(Vector2f(500, 200));
		auto s = f_temp->addComponent<SpriteComponent>();
		s->setSprite(Sprite(*(s->setTexture(p)), IntRect(0.0f, 0.0f, 150.0f, 300.0f)));
	
	/*auto resolution = makeEntity(false);
	auto t = resolution->addComponent<TextComponent>(
		"Resolution:");*/
	setLoaded(true);
}

void OptionScene::Update(const double & dt)
{
	Scene::Update(dt);
}
