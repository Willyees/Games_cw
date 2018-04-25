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
  
  rw->setView(rw->getDefaultView());
  while (!spritesStatic.empty()) {
	  rw->draw(*spritesStatic.front());
	  spritesStatic.pop();
  }
  
  //many cases, could be improved. TODO: add cases x and y are > mapWidth
  float x = view.getCenter().x;
  float y = view.getCenter().y;
  if (view.getCenter().x < rw->getSize().x / 2)
	  x = rw->getSize().x / 2;
  /*else if ((view.getCenter().x > (width -  rw->getSize().x / 2)) {
	  x = width - rw->getSize().x / 2 
  }*/
  if (view.getCenter().y < rw->getSize().y / 2)
	  y = rw->getSize().y / 2;
  /*else if ((view.getCenter().y > (height -  rw->getSize().y / 2)) {
  y = width - rw->getSize().y / 2
  }*/
  view.setCenter(x, y);
  rw->setView(view);

  while (!sprites.empty()) {
    rw->draw(*sprites.front());
    sprites.pop();
  }

  
}

void Renderer::queue(const sf::Drawable* s, const bool dynamic) { 
	if (dynamic)
		sprites.push(s);
	else
		spritesStatic.push(s);
	}