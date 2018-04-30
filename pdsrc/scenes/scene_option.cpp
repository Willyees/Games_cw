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
#include "../components/cmp_text_list.h"
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


static shared_ptr<Entity> resolution;
static shared_ptr<Entity> fullscreen;

void OptionScene::Load()
{
	auto resolution_text = makeEntity(false);
	auto t = resolution_text->addComponent<TextComponent>(
		"Resolution:");
	resolution_text->setPosition(Vector2f(200.f, 200.f));

	resolution = makeEntity(false);
	vector<string> textlist1{ "800 x 600", "1000 x 800", "1024 x 800", "1280 x 1024", "1360 x 768", "1600 x 900", "1920 x 1080" };
	auto t2 = resolution->addComponent<TextComponentList>(textlist1);
	t2->setDefault(to_string(static_cast<int>(Engine::user_preferences.video_resolution.x)) + " x " + to_string(static_cast<int>(Engine::user_preferences.video_resolution.y)));
	resolution->setPosition(Vector2f(600.0f, 200.0f));



	auto fullscreen_text = makeEntity(false);
	t = fullscreen_text->addComponent<TextComponent>(
		"Full Screen:");
	fullscreen_text->setPosition(Vector2f(200.f, 300.f));
	
	fullscreen = makeEntity(false);
	vector<string> textlist{ "ON", "OFF" };
	auto t1 = fullscreen->addComponent<TextComponentList>(textlist);
	if (Engine::user_preferences.fullscreen == 7)
		t1->setDefault("OFF");
	else
		t1->setDefault("ON");
	fullscreen->setPosition(Vector2f(600.0f, 300.0f));
	
	Texture z;
	z.loadFromFile("res/images/ezgif.com-gif-maker640widthtransparent2.png");
	shared_ptr<Entity> k = makeEntity(false);
	k->setPosition(Vector2f(200.0f,200.0f));
	auto d = k->addComponent<SpriteComponent>();
	d->setSprite(Sprite(*(d->setTexture(z)), IntRect(0, 0, 640, 480)));
	//d->getSprite().setOrigin(Vector2f(25.0f, 25.0f));
	setLoaded(true);
}

void OptionScene::Update(const double & dt)
{
	//dirty trick to change preferences for the user (better to create a component)
	if(resolution->get_components<TextComponentList>()[0]->changed)
	{ 
		string text = resolution->get_components<TextComponentList>()[0]->getText().getString();
		int index = text.find('x');
		int i1;
		int i2;
		stringstream(text.substr(0, index - 1)) >> i1;
		stringstream(text.substr(index + 1)) >> i2;
		
		Engine::user_preferences.video_resolution = Vector2f(i1, i2);
		Engine::user_preferences.changed_resolution = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::ChangeScene((Scene*)&menu);
	}
	
	if(fullscreen->get_components<TextComponentList>()[0]->changed){
		Engine::user_preferences.changed_fullscreen = true;
		if (fullscreen->get_components<TextComponentList>()[0]->getText().getString() == "ON") {
			Engine::user_preferences.fullscreen = 8;
			cout << "ON" << endl;
		}
		else {//must be "OFF" then
			cout << "OFF" << endl;
			Engine::user_preferences.fullscreen = 7;
			  
		}
	}
	Scene::Update(dt);
}
