#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_blinking.h"
#include "SFML/Graphics.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML\Audio\Music.hpp>
#include <iostream>
#include "../animatedSprite.hpp"

using namespace std;
using namespace sf;

void MenuScene::Load() {
	if (!this->theme.openFromFile("res/audio/Interlude_Boss.ogg"))
		cout << "Error: we not found music file";
	this->theme.setLoop(true);
	this->theme.setVolume(75);
	this->theme.play();
	
	Animation a1;
	Texture p;
	p.loadFromFile("res/images/BG_menuPD.png");

	auto sprite = makeEntity(false);
	auto cmp = sprite->addComponent<SpriteComponentAnimated>();
	a1.setSpriteSheet(*(cmp->addTexture(p)));
	cmp->addFrames(a1, 37, 4, 1000.0f, 410.0f, 0.0f);
	AnimatedSprite b(sf::seconds(0.20f), true, true);

	b.setOrigin(Engine::getWindowSize().x/2, Engine::getWindowSize().y/2);
	b.setPosition(Vector2f(Engine::getWindowSize().x/2, Engine::getWindowSize().y/2));

	cmp->setSprite(b);
	cmp->getSprite().setAnimation(a1);
	b.setOrigin(500.0f, 205.0f);
	cmp->addSprite("idle", b, a1);
	sprite->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2));
	
	setLoaded(true);
  
	cout << "Menu Load \n";
  {
	
    auto title = makeEntity(false);
    auto t = title->addComponent<TextComponent>(
        "Potato's Destiny");
	auto newGame = makeEntity(false);
	t = newGame->addComponent<TextComponent>(
		"New Game");
	auto shop = makeEntity(false);
	t = shop->addComponent<TextComponent>(
		"Shop");
	auto options = makeEntity(false);
	t = options->addComponent<TextComponent>(
		"Options");
	auto exit = makeEntity(false);
	t = exit->addComponent<TextComponent>(
		"Exit");
	
	newGame->setPosition(Vector2f(200.f, 200.f));
	shop->setPosition(Vector2f(200.f, 300.f));
	options->setPosition(Vector2f(200.f, 400.f));
	exit->setPosition(Vector2f(200.f, 500.f));
	
	auto pointer = makeEntity(false);
	pointer->addTag("pointer");
	pointer->addComponent<BlinkComponent>(0.5f);
	auto s = pointer->addComponent<ShapeComponent>();
	//float fontsize = t->GetFontSize();
	//hardcoding fontsize atm, because it cannot be accessed from the Update
	pointer->setPosition(newGame->getPosition() - Vector2f(20.f, - (20.f))); //Vector2f(20.f, - (fontsize + fontsize / 3) / 2) the box around the text is bigger around 1/3 of fontsize. This will set blinking element central
	s->setShape<sf::RectangleShape>(Vector2f(10.f, 10.f));
	s->getShape().setFillColor(Color::Magenta);
	s->getShape().setOrigin(5.f, 5.f);
	posMenuItems.push_back(newGame->getPosition());
	posMenuItems.push_back(shop->getPosition());
	posMenuItems.push_back(options->getPosition());
	posMenuItems.push_back(exit->getPosition());
	for (int i = 0; i < posMenuItems.size(); i++) {
		cout << posMenuItems[i] << endl;
	}
  }
  
  
  
  
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";
	static int posPoint = 0;
	static float countdown = 0.0f;
	countdown -= dt;
  
  if (sf::Keyboard::isKeyPressed(Keyboard::Return)) {//set boolean on keypressed until is not released
	  if (ents.find("pointer")[0]->getPosition() == posMenuItems[0] - Vector2f(20.f, -20.f)) {//newgame
		  Engine::ChangeScene(&level1);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[1] - Vector2f(20.f, -20.f)) {//shop
		  //Engine::ChangeScene(&shop);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[2] - Vector2f(20.f, -20.f)) {//options
		  Engine::ChangeScene(&optionScene);
	  }
	  else if (ents.find("pointer")[0]->getPosition() == posMenuItems[3] - Vector2f(20.f, -20.f)) {//exit
		  Engine::GetWindow().close();
	  }
	  
  }
  if (sf::Keyboard::isKeyPressed(Keyboard::Down)) {
	  if (posPoint < (posMenuItems.size() - 1) && countdown <= 0){
	  countdown = 0.15f;//using countdown to give time the player to press again (or will get in this if too many times, pc is fast!)
	  posPoint++;
	  cout << posPoint << endl;
	  cout << posMenuItems[posPoint] << endl;
	  ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
	  }
  }
  if (sf::Keyboard::isKeyPressed(Keyboard::Up)) {
	  if (posPoint > 0 && countdown <= 0) {
		  countdown = 0.2f;
		  posPoint--;
		  ents.find("pointer")[0]->setPosition(posMenuItems[posPoint] - Vector2f(20.f, -20.f));
		  
	  }
  }
  if (Keyboard::isKeyPressed(Keyboard::Escape)) {
	  Engine::GetWindow().close();
  }

  Scene::Update(dt);
}
