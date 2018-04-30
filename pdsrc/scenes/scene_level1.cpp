#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_life.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_follow_pos.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_powerup.h"
#include "../components/cmp_get_hurt_by_player.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include <SFML\Graphics\View.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio\Music.hpp>
#include "system_renderer.h"
#include "system_physics.h"
using namespace rapidjson;

using namespace std;
using namespace sf;

//might use weak pointer to not increase use count and let destroy the entity from the isfordelete() (ecm.cpp)
static shared_ptr<Entity> player;
static shared_ptr<Entity> score;
static shared_ptr<Texture> background_text;
static shared_ptr<Entity> masher;

void Level1Scene::UnlockNextScene() { _nextSceneUnlocked = true; }

void Level1Scene::Load() {
	/*menu.theme.stop();
	if (!this->theme.openFromFile("res/musics/Vidian_-_aether_theories_1.ogg"))
		cout << "Error: we not found music file";
	this->theme.setLoop(true);
	this->theme.setVolume(75);
	this->theme.play();*/

  cout << " Scene 1 Load" << endl;
  ls::loadLevelFileJson("res/levels/Map.json");

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, 0));//TODO:check how to use offset (before was set to ho)
 
  //background 
  {
	  background_text = make_shared<Texture>();
	  background_text->loadFromFile("res/images/kitchen background.jpg");
	  /*shared_ptr<Sprite> test = make_shared<Sprite>(*p);*/
	  Sprite s(*background_text);
	  
	  /*auto s = test->addComponent<SpriteComponent>();
	  s->setSprite(Sprite(*(s->setTexture(p))));*/
	  s.setPosition(Vector2f(0,0));
	  setBackground(s);
		//s->getShape().setOrigin(Vector2f(200.0f, 200.0f));
	  
		//test->addComponent<PlayerPhysicsComponent>(Vector2f(50.f, 50.f));

  }


  // Create player
  {
	  player = makeEntity(true);
	  player->entityType = EntityType::PLAYER;
	  player->addTag("player");//adding tag to let turret component find the player and shoot at him
	  player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));

	  Animation a_right;
	  Texture p;
	  p.loadFromFile("res/images/player_sprites/walk_right.png");

	  auto cmp = player->addComponent<SpriteComponentAnimated>();
	  a_right.setSpriteSheet(*(cmp->addTexture(p)));
	  //cmp->addFrames(a1, 41, 5, 640.0f, 480.0f, 0.0f);
	  cmp->addFrames(a_right, 41, 5, 180.0f, 135.4f, 0.0f);
	  AnimatedSprite as_right(sf::seconds(0.02f), true, true);
	  as_right.setOrigin(90.0f, 67.7f);
	  cmp->addSprite("right", as_right, a_right);
	  
	  
	  //cmp->getSprite().setOrigin(90.0f, 67.7f);//needs to set origin because physics create box using center origin
	  

	  Animation a_idle;
	  AnimatedSprite as_idle(sf::seconds(0.02f), true, true);//seconds, paused, looped
	  a_idle.setSpriteSheet(*(cmp->addTexture(p)));
	  cmp->addFrames(a_idle, 1, 5, 180.0f, 135.4f, 0.0f);
	  as_idle.setOrigin(90.0f, 67.7f);
	  cmp->addSprite("idle", as_idle, a_idle);
	  

  
	Animation a_left;
	AnimatedSprite as_left(sf::seconds(0.02f), true, true);
	Texture p1;
	p1.loadFromFile("res/images/player_sprites/walk_left_1.png");
	a_left.setSpriteSheet(*(cmp->addTexture(p1)));
	cmp->addFrames(a_left, 41, 5, 180.0f, 135.4f, 0.0f);
	as_left.setOrigin(90.0f, 67.7f);
	cmp->addSprite("left", as_left, a_left);
	

	Texture p2;
	p2.loadFromFile("res/images/player_sprites/jumpL.png");
	Animation a_air_left;
	AnimatedSprite as_air_left(sf::seconds(0.025f), true, false);
	a_air_left.setSpriteSheet(*(cmp->addTexture(p2)));
	cmp->addFrames(a_air_left, 45, 6, 180.0f, 135.4f, 0.0f);
	as_air_left.setOrigin(90.0f, 67.7f);
	cmp->addSprite("in air left", as_air_left, a_air_left);

	

	Animation a_air_right;
	Texture p3;
	AnimatedSprite as_air_right(sf::seconds(0.025f), true, false);
	p3.loadFromFile("res/images/player_sprites/jumpR.png");
	a_air_right.setSpriteSheet(*(cmp->addTexture(p3)));
	cmp->addFrames(a_air_right, 41, 5, 180.0f, 135.4f, 0.0f);
	as_air_right.setOrigin(90.0f, 67.7f);
	cmp->addSprite("in air right", as_air_right,a_air_right);

	auto phy = player->addComponent<PlayerPhysicsComponent>(Vector2f(60.0f, 90.0f));
	phy->setRestitution(0.0f);
	
	player->addComponent<LifeComponent>(3);
	//set view to center on player
	Renderer::view.reset(sf::FloatRect(player->getPosition().x, player->getPosition().y, Engine::getWindowSize().x, Engine::getWindowSize().y));
	
	}
  //score
  {
	  score = makeEntity(false);
	  score->entityType = EntityType::SCORE;
	  score->addComponent<TextComponent>("Score: " + scorePoints);
	  score->setPosition(Vector2f(Engine::getWindowSize().x - 200.0f, 20.0f));
  }
  {

  }

  // Add coins
  {
	  Texture p;
	  p.loadFromFile("res/images/coinsprite.png");
	  
	  Animation a;
	  
	  auto coins = ls::findTiles(ls::COIN);
	  
	 //setting coins using top left position (of levelsysytem coin TILE) as center for the coin sprite.
	 //using different physics component size because has pixel that are not coin only
	  for (auto c : coins){
		  
		  Vector2f pos = ls::getTilePosition(c);
		  pos += Vector2f(25.0f, 25.0f);
		  shared_ptr<Entity> coin_temp = makeEntity(true);
		  coin_temp->setPosition(pos);
		  auto s = coin_temp->addComponent<SpriteComponentAnimated>();
		  a.setSpriteSheet(*(s->addTexture(p)));//adding texture internally and giving it to the animation as well
		  s->addFrames(a, 6, 6, 50.0f, 86.0f, 0.0f);
		  AnimatedSprite b(sf::seconds(0.1f), true, true);
		  
		  b.setOrigin(25.0f, 43.0f);//needs to set origin because physics create box using center origin
		  s->addSprite("idle", b, a);
		  
		  coin_temp->entityType = EntityType::COIN;
		  coin_temp->addComponent<GetHurtByPlayerComponent>();
		  //auto phy = coin_temp->addComponent<PhysicsComponent>(false, Vector2f(40.0f,40.0f));
		  //phy->setRestitution(0.0f);
	  }
  }
  //Add Oil
  {
	  Texture p;
	  p.loadFromFile("res/images/oil.png");

	  Animation a;

	  auto oil = ls::findTiles(ls::OIL);

	  //setting coins using top left position (of levelsysytem coin TILE) as center for the coin sprite.
	  //using different physics component size because has pixel that are not coin only
	  for (auto o : oil) {

		  Vector2f pos = ls::getTilePosition(o);
		  pos += Vector2f(25.0f, 25.0f);
		  shared_ptr<Entity> oil_temp = makeEntity(true);
		  oil_temp->setPosition(pos);
		  auto s = oil_temp->addComponent<SpriteComponentAnimated>();
		  a.setSpriteSheet(*(s->addTexture(p)));//adding texture internally and giving it to the animation as well
		  s->addFrames(a, 3, 2, 50.0f, 50.0f, 0.0f);
		  AnimatedSprite b(sf::seconds(0.05f), true, true);

		  b.setOrigin(25.0f, 25.0f);//needs to set origin because physics create box using center origin
		  s->addSprite("idle", b, a);

		  oil_temp->entityType = EntityType::OIL;
		  auto phy = oil_temp->addComponent<PhysicsComponent>(false, Vector2f(50.0f, 50.0f));
		  oil_temp->addComponent<HurtComponent>();
		  phy->setRestitution(0.0f);
	  }
  }
  // Add Fertilizer
  {
	  Texture p;
	  p.loadFromFile("res/images/fertalizer.png");

	  Animation a;

	  auto fertalizer = ls::findTiles(ls::FERTALIZER);


	  for (auto f : fertalizer) {

		  Vector2f pos = ls::getTilePosition(f);
		  //pos -= Vector2f(20.0f, 20.0f);
		  shared_ptr<Entity> f_temp = makeEntity(true);
		  f_temp->setPosition(pos);
		  auto s = f_temp->addComponent<SpriteComponent>();
		  f_temp->addComponent<PowerUpComponent>();
		  s->setSprite(Sprite(*(s->setTexture(p)),IntRect(0,0,50,50)));//adding texture internally and giving it to the animation as well
		  AnimatedSprite b(sf::seconds(0.1f), true, true);


		  f_temp->entityType = EntityType::POWERUP;
		  //auto phy = f_temp->addComponent<PhysicsComponent>(false, Vector2f(40.0f, 40.0f));
		  
		  //phy->setRestitution(0.0f);
	  }
  }
  //add blade obstacle
  {
	  Texture p;
	    p.loadFromFile("res/images/blade.png");
	    //add enemy texture to "textures"
	    Animation a;
	    
	    auto blade = ls::findTiles(ls::BLADE);
		for (auto b : blade) {
			Vector2f pos = ls::getTilePosition(b);
			shared_ptr<Entity> enemy_temp = makeEntity(true);
			enemy_temp->setPosition(pos + Vector2f(25.0f, 25.0f));
			auto s = enemy_temp->addComponent<SpriteComponentAnimated>();
			s->addFrames(a, 5, 2, 150.0f, 153.0f, 0.0f);
			AnimatedSprite b(sf::seconds(0.05f), true, true);
			b.setOrigin(75.0f, 76.5f);//needs to set origin because physics create box using center origin
			a.setSpriteSheet(*(s->addTexture(p)));
			s->addSprite("idle", b, a);
			enemy_temp->entityType = EntityType::ENEMY;
			enemy_temp->addComponent<PhysicsComponent>(false, Vector2f(150.0f, 153.0f));
			enemy_temp->addComponent<HurtComponent>();
			//enemy_temp->addComponent<EnemyTurretComponent>();
		}
		
	    
	  	  
  }

  //add masher enemy 
  {
		Texture chain;
	    chain.loadFromFile("res/images/chain.png");
		
	    Animation a;
	    
	    auto masher = ls::findTiles(ls::MASHER);
		for (auto m : masher) {
			Vector2f pos = ls::getTilePosition(m);
			auto m = makeEntity(true);
			m->setPosition(pos);
			auto s = m->addComponent<SpriteComponentAnimated>();
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 64.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 86.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 132.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 171.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 203.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 244.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 263.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 263.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 284.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 336.0f));
			a.addFrame(IntRect(0.0f, 0.0f, 22.0f, 372.0f));
			/*s->addFrames(a, 11, 5, 179.0f, 435.0f, 0.0f);*/
			AnimatedSprite b(sf::seconds(0.1f), true, true);
			//b.setOrigin(89.5f, 217.5f);//needs to set origin because physics create box using center origin
			a.setSpriteSheet(*(s->addTexture(chain)));
			s->addSprite("idle", b, a);
			m->entityType = EntityType::ENEMY;
			//enemy_temp->addComponent<PhysicsComponent>(false, Vector2f(270.0f, 270.0f));

			//masherbottom
			Texture p;
			p.loadFromFile("res/images/masher_bottom.png");

			shared_ptr<Entity> masher_bottom = makeEntity(true);
			shared_ptr<SpriteComponent> s1 = masher_bottom->addComponent<SpriteComponent>();
			s1->setSprite(Sprite(*(s1->setTexture(p)), IntRect(0, 0, 126, 66)));
			s1->getSprite().setOrigin(Vector2f(63.0f, 0.0f));
			masher_bottom->addComponent<FollowPosComponent>(&(*m));
			masher_bottom->addComponent<HurtComponent>();
		}
		

  }
  
  
  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(25.f, 25.f); //offset to center
      auto e = makeEntity(true);
	  e->entityType = EntityType::WALL;
      e->setPosition(pos);
      auto phy = e->addComponent<PhysicsComponent>(false, Vector2f(50.f, 50.f));
	  phy->setRestitution(0.0f);
    }
  }
  //add physics colliders invisible walls
  {
	  auto walls = ls::findTiles(ls::INVISWALL);
	  for (auto w : walls) {
		  auto pos = ls::getTilePosition(w);
		  pos += Vector2f(25.f, 25.f); //offset to center
		  auto e = makeEntity(true);
		  e->entityType = EntityType::WALL;
		  e->setPosition(pos);
		  auto phy = e->addComponent<PhysicsComponent>(false, Vector2f(50.f, 50.f));
		  phy->setRestitution(0.0f);
	  }
  }
  //Add life
  {
	  Texture p;
	  p.loadFromFile("res/images/heart.png");
	 
	  auto life = makeEntity(false);
	  life->entityType = EntityType::LIFE;
	  life->addTag("life");
	  life->setPosition(Vector2f(Engine::getWindowSize().x - 200, 80));
	  auto s = life->addComponent<SpriteComponentRepeted>(player->get_components<LifeComponent>()[0]->getLives());
	  s->setSprite(Sprite(*(s->setTexture(p)), IntRect(0, 0, 16, 16)));

  }

  //Enemy
  {
	  
	  Texture p;
	  p.loadFromFile("res/images/TileSet1.png");
	  //add enemy texture to "textures"
	  
	  Animation a;
	  
	  auto enemies = ls::findTiles(ls::ENEMY);
	  //auto e = enemies[1];
	 for (auto e : enemies) {
		  Vector2f pos = ls::getTilePosition(e);
		  pos += Vector2f(8.0f, 8.0f);
		  shared_ptr<Entity> enemy_temp = makeEntity(true);
		  enemy_temp->setPosition(pos);
		  auto s = enemy_temp->addComponent<SpriteComponentAnimated>();

		  s->addFrames(a, 10, 5, 16.0f, 16.0f, 0.0f);
		  AnimatedSprite b(sf::seconds(0.05f), true, true);
		  s->getSprite().setOrigin(8.0f, 8.0f);//needs to set origin because physics create box using center origin
		  a.setSpriteSheet(*(s->addTexture(p)));
		  s->addSprite("idle", b, a);
		  enemy_temp->entityType = EntityType::ENEMY;
		  //enemy_temp->addComponent<PhysicsComponent>(false, Vector2f(16.0f, 16.0f));
		  enemy_temp->addComponent<EnemyTurretComponent>();
	  }
		  
  }
  //TODO: find key sprite and fix all the sizes.
  //Key
  {
	  Texture p;
	  p.loadFromFile("res/images/key.png");
	  auto keys = ls::findTiles(ls::KEY);
	  for (auto k : keys) {
		  Vector2f pos = ls::getTilePosition(k);
		  pos += Vector2f(8.0f, 8.0f);
		  shared_ptr<Entity> k = makeEntity(true);
		  k->setPosition(pos);
		  auto s = k->addComponent<SpriteComponent>();
		  s->setSprite(Sprite(*(s->setTexture(p)), IntRect(0, 0, 50, 50)));
		  s->getSprite().setOrigin(Vector2f(25.0f, 25.0f));
		  k->addComponent<GetHurtByPlayerComponent>();
		  //add component which will set scene _nextSceneUnlocked to true when destroyed or touched
	  }
  }

  {
	// Pause menu.
	  _pausetex.loadFromFile( "res/images/pause.png" );
	  _playtex.loadFromFile( "res/images/play.png" );

	  const int border = 15;
	  const int size = 100;
	  _pauserect.left = border;
	  _pauserect.top = border;
	  _pauserect.height = size;
	  _pauserect.width = size;
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
	// Use this to force player to let go of button before the button effect kicks in again.
	static bool mouse_released = true; 
	// Check if pause button was pressed.
	if(Mouse::isButtonPressed( Mouse::Left ) )
	{
		if( mouse_released )
		{
			Vector2i mpos = Mouse::getPosition( Engine::GetWindow() );
			if( _pauserect.contains( mpos ) )
			{
				_paused = !_paused;
			}
			mouse_released = false;
		}
	}
	else { mouse_released = true; }
	
  if (ls::getTileAt(player->getPosition()) == ls::PORTAL && _nextSceneUnlocked == true) {
	 
    //
  }

  if (Keyboard::isKeyPressed(Keyboard::Num3)) {//almost make it invicible
	  player->get_components<LifeComponent>()[0]->increaseLives();
	  
	 
	 return;
  }
  float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
  float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
  static float timer = 0;
  timer += dt;
  if(timer > 5.0f) {
	cout << x << endl;
	cout << y << endl;
	timer = 0.0f;
  }

  /*if (coin->get_components<PhysicsComponent>()[0]->isTouching(*player->get_components<PlayerPhysicsComponent>()[0])) {
	  cout << "yes they are !" << endl;
  }*/
  //not the cleanest way to update the score points
  if (!player->isAlive()) {
	  //change scene and unload everything
	  Engine::ChangeScene((Scene*)&gameOver);
	  return;
  }
  score->get_components<TextComponent>()[0]->SetText("Score " + std::to_string(scorePoints));
  
  Renderer::view.setCenter(player->getPosition().x, player->getPosition().y);
  
  if( !_paused )
  {
	  Scene::Update( dt );
  }
  
}

void Level1Scene::Render() {
  if (_background != nullptr) {
	  Engine::GetWindow().draw(*_background);
  }

  auto& window = Engine::GetWindow();

  // Keep game view to be restored
  auto oldview = window.getView();

  // Setup a separate view for GUI rendering.
  sf::View GUIView;
  GUIView.setSize( Vector2f( window.getSize() ) );
  window.setView( GUIView );
  if( _paused )
	  _pausesprite.setTexture( _playtex );
  else _pausesprite.setTexture( _pausetex );
  Vector2f pauseSourceSize = Vector2f( _pausetex.getSize() );
  Vector2f pauseTargetSize = Vector2f( _pauserect.width, _pauserect.height );
  Vector2f pausescale = Vector2f( pauseTargetSize.x / pauseSourceSize.x, pauseTargetSize.y / pauseSourceSize.y );
  _pausesprite.setPosition( Vector2f( _pauserect.left - _pauserect.width, _pauserect.top + _pauserect.height ) );
  _pausesprite.setScale( pausescale );
  window.draw( _pausesprite );

  // Restore game view after rendering GUI.
  window.setView( oldview );

  Scene::Render();
  ls::render( window );
  
}

void Level1Scene::collisionHandler(Entity * entityA, Entity * entityB)
{
	switch (entityA->entityType)
	{
	case EntityType::COIN:
		switch (entityB->entityType)
		{
		case EntityType::PLAYER:
			entityA->setForDelete();
			Engine::getActiveScene()->addScore(10);
		default:
			break;
		}
		break;
	case EntityType::BULLET:
		switch (entityB->entityType)
		{
		case EntityType::PLAYER:
			entityB->get_components<LifeComponent>().front()->reduceLives();
			entityA->get_components<PhysicsComponent>()[0]->dampen(Vector2f(0.0f, 0.0f));
			entityA->setForDelete();
			break;
		case EntityType::WALL:
			entityA->setForDelete();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
		
	switch (entityB->entityType)
	{
	case EntityType::COIN:
		switch (entityB->entityType)
		{
		case EntityType::PLAYER:
			entityB->setForDelete();
			Engine::getActiveScene()->addScore(10);
			break;

			default:
				break;
		}
		break;
		
	case EntityType::BULLET:
		switch (entityA->entityType)
		{
		case EntityType::PLAYER:
			entityA->get_components<LifeComponent>().front()->reduceLives();
			entityB->get_components<PhysicsComponent>()[0]->dampen(Vector2f(0.0f, 0.0f));
			entityB->setForDelete();
			
			break;
		case EntityType::WALL:
			entityB->setForDelete();
			break;
		default:
			break;
		}
		break;
	
	default:
		break;
	}
		
}

