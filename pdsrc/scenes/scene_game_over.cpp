#include "scene_game_over.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "scene_menu.h"
#include "system_renderer.h"
#include "../game.h"
#include <SFML\Audio\Music.hpp>

using namespace std;
using namespace sf;


void GameOverScene::Load()
{
	cout << "Game over scene load" << endl;
	//text game over
	{
		auto text_game = makeEntity(false);
		text_game->addComponent<TextComponent>("-------------GAME OVER!-------");

	}
	setLoaded(true);
	
	if (!this->theme.openFromFile("res/audio/Gameover.ogg"))
		cout << "Error: we not found music file";
	this->theme.setLoop(true);
	this->theme.setVolume(75);
	this->theme.play();
}

void GameOverScene::UnLoad()
{
	Scene::UnLoad();
}

void GameOverScene::Update(const double & dt)
{
	float timer = 0;
	timer += dt;
	if(timer > 3.0f) 
		Engine::ChangeScene((Scene*)&menu);

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::GetWindow().close();
	}
	Scene::Update(dt);
}
