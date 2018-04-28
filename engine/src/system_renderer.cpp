#include "system_renderer.h"
#include <queue>
#include "engine.h"
#include <LevelSystem.h>
#include <iostream>


using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static queue<const Drawable*> spritesStatic;
static RenderWindow* rw;


void Renderer::initialise(sf::RenderWindow& r) { 
	rw = &r; 
	view = rw->getDefaultView();
}

void Renderer::shutdown() {
  while (!sprites.empty())
    sprites.pop();
}

void Renderer::update(const double&) {}

void Renderer::render() {
  if (rw == nullptr) {
    throw("No render window set! ");
  }
  //set view size in case window size has been changed
  view.setSize(Vector2f(Engine::getWindowSize()));

  Vector2f view_temp_center = view.getCenter();
  view.setCenter(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2));
  rw->setView(view);

  while (!spritesStatic.empty()) {
	  rw->draw(*spritesStatic.front());
	  spritesStatic.pop();
  }
  view.setCenter(view_temp_center);
  
  //many cases, could be improved. TODO: add cases x and y are > mapWidth
  float x = view.getCenter().x;
  float y = view.getCenter().y;
  if (view.getCenter().x < rw->getSize().x / 2)
	  x = rw->getSize().x / 2;
  else if ((view.getCenter().x > (ls::getWidth() -  rw->getSize().x / 2))) {
	  x = ls::getWidth() - rw->getSize().x / 2;
  }
  //checking if the map is higher than the screen first
  if(ls::getHeight() > view.getSize().y) {
	  if (view.getCenter().y < rw->getSize().y / 2)
		  y = rw->getSize().y / 2;
	  else if ((view.getCenter().y > (ls::getHeight() -  rw->getSize().y / 2))) {
		  y = ls::getHeight() - rw->getSize().y / 2;
	  }
  }
  else {
	  y = ls::getHeight() / 2;
  }
  view.setCenter(x, y);
  rw->setView(view);

  while (!sprites.empty()) {
    rw->draw(*sprites.front());
    sprites.pop();
  }
  view_temp_center = Vector2f(0.0f, 0.0f);
  
}

void Renderer::queue(const sf::Drawable* s, const bool dynamic) { 
	if (dynamic)
		sprites.push(s);
	else
		spritesStatic.push(s);
	}