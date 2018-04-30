#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_text.h"
#include "../components/cmp_life.h"
#include "../components/cmp_enemy_turret.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include <SFML\Graphics\View.hpp>
#include <SFML\Audio\Music.hpp>
#include "system_renderer.h"
#include "system_physics.h"
#include "C:\Users\Tomer\source\repos\Games_cw\engine\Sound\Sound.h"
#include "C:\Users\Tomer\source\repos\Games_cw\engine\Sound\SoundSys.h"

using namespace std;
using namespace sf;

//might use weak pointer to not increase use count and let destroy the entity from the isfordelete() (ecm.cpp)
static shared_ptr<Entity> player;
static shared_ptr<Entity> score;


void Level1Scene::Load() {
	menu.theme.stop();
	std::shared_ptr<SoundSys> s = nullptr;
	s = Factory::CreateSound();
	/*if (!this->theme.openFromFile("res/musics/mainTheme.ogg"))
		cout << "Error: we not found music file";*/
	try 
	{
		s->SetMusicBuffer("res/musics/mainTheme.ogg");
	}
	catch (std::string Error_message)
	{
		if (Error_message == "Sound File Error")
			std::cout << "Something went wrong with the sound files";
	}
	this->theme.setBuffer(s->reSfx()->at(0).sbuf);
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
	player->addTag("player");//adding tag to let turret component find the player and shoot at him
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	
	Animation a_right;
	Texture p;
	p.loadFromFile("res/images/ezgif.com-gif-maker150widthtransparent.png");

	auto cmp = player->addComponent<SpriteComponentAnimated>();
	a_right.setSpriteSheet(*(cmp->addTexture(p)));
	//cmp->addFrames(a1, 41, 5, 640.0f, 480.0f, 0.0f);
	cmp->addFrames(a_right, 41, 5, 150.0f, 113.0f, 0.0f);
	AnimatedSprite b(sf::seconds(0.05f), true, true);
	b.setPosition(Vector2f(300.0f, 300.0f));
	cmp->setSprite(b);
	cmp->getSprite().setAnimation(a_right);
	cmp->getSprite().setOrigin(75.0f, 56.0f);//needs to set origin because physics create box using center origin
	cmp->addAnimation("right", a_right);
    
	Animation a_idle;
	a_idle.setSpriteSheet(*(cmp->addTexture(p)));
	cmp->addFrames(a_idle, 1, 1, 150.0f, 113.0f, 0.0f);
	cmp->addAnimation("idle", a_idle);

	Animation a_left;
	Texture p1;
	p1.loadFromFile("res/images/ezgif.com-gif-maker150widthtransparentleft.png");
	a_left.setSpriteSheet(*(cmp->addTexture(p1)));
	cmp->addFrames(a_left, 41, 5, 150.0f, 113.0f, 0.0f);
	cmp->addAnimation("left", a_left);
	
	Animation a_air_left;
	a_air_left.setSpriteSheet(*(cmp->addTexture(p1)));
	cmp->addFrames(a_air_left, 1, 1, 150.0f, 113.0f, 0.0f);
	cmp->addAnimation("in air left", a_air_left);

	Animation a_air_right;
	a_air_right.setSpriteSheet(*(cmp->addTexture(p)));
	cmp->addFrames(a_air_right, 1, 1, 150.0f, 113.0f, 0.0f);
	cmp->addAnimation("in air right", a_air_right);

	player->addComponent<PlayerPhysicsComponent>(Vector2f(50.0f, 75.f));
	player->addComponent<LifeComponent>(3);
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
	  p.loadFromFile("res/images/coinsprite.png");
	  
	  Animation a;
	  
	  auto coins = ls::findTiles(ls::COIN);
	  
	 
	  for (auto c : coins){
		  
		  Vector2f pos = ls::getTilePosition(c);
		  pos += Vector2f(8.0f, 8.0f);
		  shared_ptr<Entity> coin_temp = makeEntity(true);
		  coin_temp->setPosition(pos);
		  auto s = coin_temp->addComponent<SpriteComponentAnimated>();
		  a.setSpriteSheet(*(s->addTexture(p)));//adding texture internally and giving it to the animation as well
		  s->addFrames(a, 6, 6, 116.0f, 200.0f, 0.0f);
		  AnimatedSprite b(sf::seconds(0.1f), true, true);
		  s->setSprite(b);
		  s->getSprite().setOrigin(58.0f, 100.0f);//needs to set origin because physics create box using center origin
		 
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
		  enemy_temp->addComponent<EnemyTurretComponent>();
		  

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
	  _paused = false;
	  _pausetex.loadFromFile( "res/images/pause.png" );
	  _playtex.loadFromFile( "res/images/play.png" );
	  _pausesprite.setTexture( _pausetex );

	  const int border = 15;
	  const int size = 100;
	  _pauserect.left = border;
	  _pauserect.top = border;
	  _pauserect.height = size;
	  _pauserect.width = size;

	  // Reset button.
	  _resettex.loadFromFile( "res/images/restart.png" );
	  _resetsprite.setTexture( _resettex );
	  
	  _resetrect.left = border + size + border;
	  _resetrect.top = border;
	  _resetrect.height = size;
	  _resetrect.width = size;

	  // Back to menu button.
	  _backtex.loadFromFile( "res/images/back.png" );
	  _backsprite.setTexture( _backtex );

	  _backrect.left = border + size + border + size + border;
	  _backrect.top = border;
	  _backrect.height = size;
	  _backrect.width = size;


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
				Engine::setPhysicsPause( _paused );
				if( _paused )
					_pausesprite.setTexture( _playtex, true );
				else _pausesprite.setTexture( _pausetex, true );

			}

			if( _resetrect.contains( mpos ) )
			{
				// Restart game.
				Engine::ChangeScene( &level1 );
				return;
			}

			if( _backrect.contains( mpos ) )
			{
				Engine::ChangeScene( &menu );
				return;
			}

			mouse_released = false;
		}
	}
	else { mouse_released = true; }

	
  if (ls::getTileAt(player->getPosition()) == ls::END) {
	 
    //
  }
  if (Keyboard::isKeyPressed(Keyboard::Num3)) {
	 // player->get_components<LifeComponent>()[0]->increaseLives();
	 Engine::ChangeScene((Scene*)&gameOver);
	 return;
  }
  /*if (coin->get_components<PhysicsComponent>()[0]->isTouching(*player->get_components<PlayerPhysicsComponent>()[0])) {
	  cout << "yes they are !" << endl;
  }*/
  //not the cleanest way to update the score points
  if (!player->isAlive()) {
	  //change scene and unload everything
	  Engine::GetWindow().close();
  }
  score->get_components<TextComponent>()[0]->SetText("Score " + std::to_string(scorePoints));
  
  Renderer::view.setCenter(player->getPosition().x, player->getPosition().y);
  
  Scene::Update(dt);
  
}

// Small helper that scales and moves a sprite to a target rectangle and renders it to screen.
void drawSpriteTo( sf::RenderWindow* window, Sprite* sprite, IntRect target )
{
	Vector2f sourceSize = Vector2f( sprite->getTexture()->getSize() );
	Vector2f scale = Vector2f( target.width / sourceSize.x, target.height / sourceSize.y );
	sprite->setPosition( Vector2f( target.left + target.width, target.top + target.height * 2 ) );
	sprite->setScale( scale );
	window->draw( *sprite );
}

void Level1Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();

  ls::render( window );


  // Keep game view to be restored
  auto oldview = window.getView();

  // Setup a separate view for GUI rendering.
  // This makes sure that the GUI positions are not in world position but relative to
  // The corner of the screen.
  sf::View GUIView;
  GUIView.setSize( Vector2f( window.getSize() ) );
  window.setView( GUIView );

  // Render GUI buttons.
  drawSpriteTo( &window, &_pausesprite, _pauserect );
  drawSpriteTo( &window, &_resetsprite, _resetrect );
  drawSpriteTo( &window, &_backsprite, _backrect );

  // Restore game view after rendering GUI.
  window.setView( oldview );

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
		default:
			break;
		}
		break;
	case EntityType::BULLET:
		switch (entityB->entityType)
		{
		case EntityType::PLAYER:
			entityB->get_components<LifeComponent>().front()->reduceLives();
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

