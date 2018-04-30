#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define ls LevelSystem

class jsonHelper;

struct json {
	std::string texture;
	float tileWidth_tileset;
	float tileHeight_tileset;
	int columns_map;
	int rows_map;
	float tileHeight_map;
	float tileWidth_map;
	int rows_tileset;
	int columns_tileset;
	std::vector<int> data;
};

class LevelSystem {
public:
  static void loadLevelFileJson(const std::string& path);
  static void loadLevelFile(const std::string&, float tileSize = 10.0f);
  static void unload();
  static void render(sf::RenderWindow& window);


  enum TILES {
    EMPTY,
    START,
    KEY,
    WALL,
	COIN,
    ENEMY,
	BLADE,
	MASHER,
	FERTALIZER,
	OIL,
	PORTAL,
	UNKNOWN,
	INVISWALL,

  };

  static TILES getTile(sf::Vector2ul);

  static TILES getTileAt(sf::Vector2f);

  static bool isOnGrid(sf::Vector2f);

  static float getWidth();

  static float getHeight();

  static sf::Vector2f getTilePosition(sf::Vector2ul);

  static std::vector<sf::Vector2ul> findTiles(TILES);

  static void setOffset(const sf::Vector2f& _offset);

  static const sf::Vector2f& getOffset();

  static float getTileSize();

  static jsonHelper* helper;//might have to set protected and access its methods from getters 
protected:
  
  static sf::Texture _texture;
static std::unique_ptr<TILES[]> _tiles;
  //TODO: just use this later:
  
  static size_t _width;
  static size_t _height;
  static sf::Vector2f _offset;

  const static std::string _spriteSheet;

  static std::vector<std::shared_ptr<sf::RectangleShape>> _sprites;

  //static void buildSprites(bool optimise = true);
  static void buildSprites();

  static float _tileSize; // for rendering
  
private:
  LevelSystem() = delete;

  ~LevelSystem() = delete;
};

struct limitElementTexture {
	int limits[2];
	ls::TILES types;
	limitElementTexture(int first, int last, ls::TILES type) {
		limits[0] = first;
		limits[1] = last;
		types = type;
	}
};

class jsonHelper {
public:
	json _json;
	const std::vector<limitElementTexture> limitsElementsTexture = 
	{
	{limitElementTexture(0,0, ls::EMPTY) },
	{ limitElementTexture(1,6, ls::WALL) },
	
	{limitElementTexture(7,7, ls::START)},
	{ limitElementTexture(8,8, ls::ENEMY)},
	{ limitElementTexture(9,9, ls::COIN) },
	{limitElementTexture(10,10, ls::BLADE)},
	{ limitElementTexture(11,11, ls::MASHER) },
	{ limitElementTexture(12,12, ls::FERTALIZER) },
	{limitElementTexture(13,13, ls::KEY)},
	{ limitElementTexture(14,14, ls::PORTAL) },
	{ limitElementTexture(15,15, ls::OIL) },
	{ limitElementTexture(16,17, ls:: INVISWALL) },

	
	};
	
	sf::IntRect getIntRect(size_t gid);
	size_t getGidAtPos(sf::Vector2ul);
	size_t getGidAtPos(sf::Vector2f);
	sf::Vector2f getPosTileinMap(int pos);
	json& getJson(std::string path);
	std::vector<ls::TILES> createTileList();
};