
#include "cmp_sprite.h"
#include "system_renderer.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

sf::Texture * SpriteComponent::setTexture(sf::Texture & texture)
{
	*_texture = texture;
	return &(*_texture);
}

//sprite static
SpriteComponent::SpriteComponent(Entity* p)
	: Component(p), _sprite(make_shared<sf::Sprite>()), _texture(make_shared<sf::Texture>()) {}

void SpriteComponent::update(double dt) {
	_sprite->setPosition(_parent->getPosition());
	_sprite->setRotation(_parent->getRotation());
}

void SpriteComponent::render() { Renderer::queue(_sprite.get(), _parent->isDynamic()); }


//shape
void ShapeComponent::update(double dt) {
	_shape->setPosition(_parent->getPosition());
	_shape->setRotation(_parent->getRotation());

}

void ShapeComponent::render() {

	Renderer::queue(_shape.get(), _parent->isDynamic());
}

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
	: Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }

//sprite animated


SpriteComponentAnimated::SpriteComponentAnimated(Entity * p)
	: Component(p), _currentSprite(make_shared<AnimatedSprite>()), _texture(make_shared<vector<Texture>>()) {
	_texture->reserve(10);//reserving 10 spaces to avoid the reallocation of the texture inside of vector, leading to different pointers for the textures
	_animations.reserve(10);
}

void SpriteComponentAnimated::update(double dt)
{
	sf::Time frameTime = sf::seconds(dt);
	_currentSprite->setPosition(_parent->getPosition());
	_currentSprite->setRotation(_parent->getRotation());

	if (state != _parent->getState()) {//using state var instead of checking with the <map>.find() for faster check
		
		if (_sprites.find(_parent->getState()) == _sprites.end() || _parent->getState() == "none")
		{
			//cout << "state in the parent does not exists in the animations map or no state" << endl;
			return;
		}
		state = _parent->getState();
		setSprite(_sprites.at(state));
		//_sprite->setAnimation(*_currentAnimation);
		
		_currentSprite->play();
	}
	

	_currentSprite->update(frameTime);
}

void SpriteComponentAnimated::render()
{
	Renderer::queue(_currentSprite.get(), _parent->isDynamic());
}

Texture* SpriteComponentAnimated::addTexture(sf::Texture & texture)
{
	_texture->push_back(texture);
	return &(_texture->back());
}

AnimatedSprite & SpriteComponentAnimated::getSprite() const
{
	return *_currentSprite;
}

//atm works on a frameset which starts on a new line -> x=0 and it is square TODO:modify to work on starting different positions
void SpriteComponentAnimated::addFrames(Animation& a,int frames, int rowlength, float width, float height, float initHeight) {
	
	for (int i = 0; i < frames; i++) {
		int x = i % rowlength;
		int y = (int)i / rowlength;
		a.addFrame(sf::IntRect(x * width, y * height + initHeight, width, height));
	}
	
}

void SpriteComponentAnimated::addSprite(std::string key, AnimatedSprite sprite, Animation animation)
{
	_animations.push_back(animation);
	sprite.setAnimation(_animations.at(_animations.size() - 1));
	std::pair<std::map<std::string, AnimatedSprite>::iterator, bool> ret;
	ret = _sprites.insert({ key, sprite });
	if (ret.second == false) {
		cout << "element " + key + " already existed" << endl;
	}
}

SpriteComponentRepeted::SpriteComponentRepeted(Entity * p, int repetition)
	: Component(p), _repetition(repetition), _sprite(make_shared<std::vector<sf::Sprite>>())
	, _texture(make_shared<Texture>())
{
	for (int i = 0; i < _repetition; i++) {
		_sprite->push_back(sf::Sprite());
	}
	
}

void SpriteComponentRepeted::update(double dt)
{
	for(int i = 0; i < _repetition; i++) {
		_sprite->at(i).setPosition(_parent->getPosition());
		_sprite->at(i).setRotation(_parent->getRotation());
	}
}

void SpriteComponentRepeted::render()
{
	for(int i = 0; i < _repetition; i++){
		float x = _parent->getPosition().x + _sprite->at(i).getLocalBounds().width * i + i*5.0f;
		float y = _parent->getPosition().y;
		_sprite->at(i).setPosition(x, y);
		Renderer::queue(&_sprite->at(i), _parent->isDynamic());
	}
}

Texture* SpriteComponentRepeted::setTexture(Texture & texture)
{
	*_texture = texture;
	return &(*_texture);
}

//sf::Sprite& SpriteComponentRepeted::getSprite() const { return *_sprite; }
