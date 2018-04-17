#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
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

static shared_ptr<Entity> player;
static shared_ptr<Entity> score;


void Level1Scene::Load() {
	menu.theme.stop();
	if (!this->theme.openFromFile("res/musics/mainTheme.ogg"))
		cout << "Error: we not found music file";
	this->theme.setLoop(true);
	this->theme.setVolume(75);
	this->theme.play();

  cout << " Scene 1 Load" << endl;
  ls::loadLevelFileJson("res/levels/untitled100.json");

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, 0));//TODO:check how to use offset (before was set to ho)
 
  // Create player
  {
    player = makeEntity(true);
	player->entityType = EntityType::PLAYER;
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    s->getShape().setFillColor(Color::Magenta);
    s->getShape().setOrigin(10.f, 15.f);
	

    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
	
	//set view to center on player
	Renderer::view.reset(sf::FloatRect(player->getPosition().x, player->getPosition().y, 1280.0f, 800.f));
	
  }
  //score
  {
	  score = makeEntity(false);
	  score->entityType = EntityType::SCORE;
	  score->addComponent<TextComponent>("Score: " + scorePoints);
	  score->setPosition(Vector2f(Engine::getWindowSize().x - 200.0f, 20.0f));
  }

  // Add coins
  {
	  Texture p;
	  p.loadFromFile("res/images/TileSet1.png");
	  
	  Animation a;
	  
	  auto coins = ls::findTiles(ls::COIN);
	  
	 
	  for (auto c : coins){
		  
		  Vector2f pos = ls::getTilePosition(c);
		  pos += Vector2f(8.0f, 8.0f);
		  shared_ptr<Entity> coin_temp = makeEntity(true);
		  coin_temp->setPosition(pos);
		  auto s = coin_temp->addComponent<SpriteComponentAnimated>();
		  a.setSpriteSheet(*(s->addTexture(p)));//adding texture internally and giving it to the animation as well
		  s->addFrames(a, 5, 5, 16.0f, 16.0f, 0.0f);
		  AnimatedSprite b(sf::seconds(0.05f), true, true);
		  s->setSprite(b);
		  s->getSprite().setOrigin(8.0f, 8.0f);//needs to set origin because physics create box using center origin
		 
		  s->addAnimation("idle", a);
		  coin_temp->entityType = EntityType::COIN;
		  coin_temp->addComponent<PhysicsComponent>(false, Vector2f(16.0f,16.0f));
		  
		  
	  }
  }

  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(8.f, 8.f); //offset to center
      auto e = makeEntity(true);
	  e->entityType = EntityType::WALL;
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(16.f, 16.f));
    }
  }
  //Add life
  {
	  Texture p;
	  p.loadFromFile("res/images/heart.png");
	 
	  auto life = makeEntity(false);
	  life->entityType = EntityType::LIFE;
	  life->setPosition(Vector2f(Engine::getWindowSize().x - 200, 80));
	  auto s = life->addComponent<SpriteComponentRepeted>(3);
	  s->setSprite(Sprite(*(s->setTexture(p)), IntRect(0, 0, 16, 16)));

  }

  //Enemy
  {
	  
	  Texture p;
	  p.loadFromFile("res/images/TileSet1.png");
	  //add enemy texture to "textures"
	  
	  Animation a;
	  //a.setSpriteSheet(textures->at(0));
	  auto enemies = ls::findTiles(ls::ENEMY);
	  for (auto e : enemies) {
		  Vector2f pos = ls::getTilePosition(e);
		  pos += Vector2f(8.0f, 8.0f);
		  shared_ptr<Entity> enemy_temp = makeEntity(true);
		  enemy_temp->setPosition(pos);
		  auto s = enemy_temp->addComponent<SpriteComponentAnimated>();

		  s->addFrames(a, 10, 5, 16.0f, 16.0f, 0.0f);
		  AnimatedSprite b(sf::seconds(0.05f), true, true);
		  s->setSprite(b);
		  s->getSprite().setOrigin(8.0f, 8.0f);//needs to set origin because physics create box using center origin
		  a.setSpriteSheet(*(s->addTexture(p)));
		  s->addAnimation("idle", a);
		  enemy_temp->entityType = EntityType::ENEMY;
		  enemy_temp->addComponent<PhysicsComponent>(false, Vector2f(16.0f, 16.0f));
		  
		  

	  }
  }

  //Simulate long loading times
  //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;
  
  setLoaded(true);
}

void Level1Scene::UnLoad() {

  this->theme.stop();
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {
	
  if (ls::getTileAt(player->getPosition()) == ls::END) {
	 
//    Engine::ChangeScene((Scene*)&level2);
  }
  /*if (coin->get_components<PhysicsComponent>()[0]->isTouching(*player->get_components<PlayerPhysicsComponent>()[0])) {
	  cout << "yes they are !" << endl;
  }*/
  //not the cleanest way to update the score points
  score->get_components<TextComponent>()[0]->SetText("Score " + std::to_string(scorePoints));
  
  Renderer::view.setCenter(player->getPosition().x, player->getPosition().y);
  
  Scene::Update(dt);
  
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
