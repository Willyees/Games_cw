#pragma once

#include <map>
#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../AnimatedSprite.hpp"

class SpriteComponentAnimated : public Component {
protected:
	Animation* _currentAnimation;
	std::shared_ptr<AnimatedSprite> _currentSprite;
	std::vector<Animation> _animations;
	std::map<std::string, AnimatedSprite> _sprites;
	std::shared_ptr<std::vector<sf::Texture>> _texture;
	std::shared_ptr<std::vector<sf::Texture>> _texture_temp;
	std::string state;
public:
	SpriteComponentAnimated() = delete;

	explicit SpriteComponentAnimated(Entity* p);
	void update(double dt) override;
	void render() override;
	sf::Texture* addTexture(sf::Texture& texture);
	void swapTextures(std::vector<sf::Texture>);
	
	AnimatedSprite& getSprite() const;
	void addSprite(std::string key, AnimatedSprite sprite, Animation animation);
	void addFrames(Animation& a, int frames, int rowlength, float width, float height, float initHeight);
	template <typename... Targs> void setSprite(Targs... params) {
		_currentSprite.reset(new AnimatedSprite(params...));
	}
};

class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;

public:
  SpriteComponent() = delete;
  sf::Texture* setTexture(sf::Texture& texture);
  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
  void render() override;

  sf::Sprite& getSprite() const;
  
  template <typename... Targs> void setSprite(Targs... params) {
    _sprite.reset(new sf::Sprite(params...));
  }
};

class SpriteComponentRepeted : public Component {
protected:
	std::shared_ptr<std::vector<sf::Sprite>> _sprite;
	std::shared_ptr<sf::Texture> _texture;
	int _repetition; std::vector<sf::Sprite>* a = new std::vector<sf::Sprite>();

public:
	explicit SpriteComponentRepeted(Entity* p, int repetition);
	void update(double dt) override;
	void render() override;
	void decreaseRep();
	void setRep( int rep );

	sf::Texture* setTexture(sf::Texture& texture);
	//sf::Sprite& getSprite() const;
	template <typename... Targs> void setSprite(Targs... params) {
		Sprite s(params...);
		_sprite.reset(new std::vector<sf::Sprite>());
		for(int i = 0; i < _repetition; i++) {
			_sprite->push_back(s);
		}
	}
};

class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;
 
public:
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  void render() override;
  sf::Shape& getShape() const;
  template <typename T, typename... Targs> void setShape(Targs... params) {
    _shape.reset(new T(params...));
  }
};