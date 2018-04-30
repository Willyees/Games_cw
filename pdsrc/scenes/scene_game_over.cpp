#include "scene_game_over.h"
#include "../components/cmp_text.h"
#include "engine.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "scene_menu.h"
#include "system_renderer.h"

using namespace std;
using namespace sf;
MenuScene menu1;

void GameOverScene::Load()
{
	cout << "Game over scene load" << endl;
	//text game over
	{
		auto text_game = makeEntity(false);
		text_game->addComponent<TextComponent>("GAME OVER!");
		Engine::Start(1200, 800, "TRY AGAIN?", &menu1);

	}
	setLoaded(true);
}

void GameOverScene::UnLoad()
{
	Scene::UnLoad();
}

void GameOverScene::Update(const double & dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Engine::GetWindow().close();
	}
	Scene::Update(dt);
}
