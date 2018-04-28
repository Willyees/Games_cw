#include "LevelSystem.h"
#include <fstream>
#include <iostream>


using namespace std;
using namespace sf;
using namespace rapidjson;

Texture LevelSystem::_texture;
jsonHelper* LevelSystem::helper = new jsonHelper();

std::unique_ptr<LevelSystem::TILES[]> LevelSystem::_tiles;

size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(50.f);
Vector2f LevelSystem::_offset(0.0f, 0.0f);
// Vector2f LevelSystem::_offset(0,0);

vector<std::shared_ptr<sf::RectangleShape>> LevelSystem::_sprites;


void LevelSystem::loadLevelFileJson(const std::string & path)
{
	helper->getJson(path);
	_width = helper->_json.columns_map;
	_height = helper->_json.rows_map;

	_texture.loadFromFile(helper->_json.texture);
	vector<TILES> temp_tiles = *(helper->createTileList());
	
	_tiles = std::make_unique<TILES[]>(helper->_json.columns_map * helper->_json.rows_map);
	
	std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
	/*for (int i = 0; i < 400; i++) {
		cout << _tiles[i] << endl;
	}*/
	
	
			//shared_ptr<RectangleShape> _sprite = make_shared<RectangleShape>();
			//_sprite->setTexture(&spritesheet);
			//_sprite->setTextureRect(sf::IntRect(x, y, tileWidth, tileHeight));
			//
			//int actual_col_map = posMap % mapWidth;
			//int actual_row_map = posMap / mapWidth;
			//
			//int map_x = actual_col_map * tileWidth_map;
			//int map_y = actual_row_map * tileHeight_map;
			//_sprite->setPosition(Vector2f(map_x, map_y));
			//_sprite->setSize(Vector2f(tileWidth, tileHeight));//have to specify size on rectangle (differently from a sprite)
			//_spritess.push_back(_sprite);
	cout << "Level " << path << " Loaded. Width: " << helper->_json.columns_map << "Height: " << helper->_json.rows_map << std::endl;
	buildSprites();
		
}

	
	




//void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {

//  _tileSize = tileSize;
//  size_t w = 0, h = 0;
//  string buffer;
//
//  // Load in file to buffer
//  ifstream f(path);
//  if (f.good()) {
//    f.seekg(0, std::ios::end);
//    buffer.resize(f.tellg());
//    f.seekg(0);
//    f.read(&buffer[0], buffer.size());
//    f.close();
//  } else {
//    throw string("Couldn't open level file: ") + path;
//  }
//
//  std::vector<Tile> temp_tiles;
//  int widthCheck = 0;
//  for (int i = 0; i < buffer.size(); ++i) {
//    const char c = buffer[i];
//    if (c == '\n') { // newline
//      if (w == 0) {  // if we haven't written width yet
//        w = i;       // set width
//      } else if (w != (widthCheck - 1)) {
//        throw string("non uniform width:" + to_string(h) + " ") + path;
//      }
//      widthCheck = 0;
//      h++; // increment height
//    } else {
//      temp_tiles.push_back((Tile)c);
//    }
//    ++widthCheck;
//  }
//
//  if (temp_tiles.size() != (w * h)) {
//    throw string("Can't parse level file") + path;
//  }
//  _tiles = std::make_unique<Tile[]>(w * h);
//  _width = w; // set static class vars
//  _height = h;
//  std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
//  cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
//  buildSpritess();
//}


void LevelSystem::buildSprites() {
	//not optimizing atm
	_sprites.clear();

	struct tp {
		sf::Vector2f pos;
		sf::Vector2f size;
		sf::IntRect rect;
	};
	vector<tp> tps;
	for (size_t y = 0; y < helper->_json.rows_map; ++y) {
		for (size_t x = 0; x < helper->_json.columns_map; ++x) {
			int gid = helper->_json.data.at(helper->_json.columns_map*y + x);
			ls::TILES t = getTile({ x,y });
			if (t == EMPTY /*|| t == COIN || t == ENEMY || t == START*/) {
				continue;
			}
			
			tps.push_back({ 
				getTilePosition({ x, y }),
				Vector2f(helper->_json.tileWidth_tileset, helper->_json.tileHeight_map), 
				helper->getIntRect(gid) });
		}
	}
	const auto nonempty = tps.size();
	//const auto nonempty = tps.size();
	//const auto tls = Vector2f(_tileSize, _tileSize);
	//  // If tile of the same type are next to each other,
	//  // We can use one large sprite instead of two.
	//  if (nonempty) {
	//
	//    vector<tp> tpo;
	//    tp last = tps[0];
	//    size_t samecount = 0;
	//
	//    for (size_t i = 1; i < nonempty; ++i) {
	//      // Is this tile compressible with the last?
	//      bool same = ((tps[i].pos.y == last.pos.y) &&
	//                   (tps[i].pos.x == last.pos.x + (tls.x * (1 + samecount))) &&
	//                   (tps[i].size == last.size));
	//      if (same) {
	//        ++samecount; // Yes, keep going
	//        // tps[i].c = Color::Green;
	//      } else {
	//        if (samecount) {
	//          last.size.x = (1 + samecount) * tls.x; // Expand tile
	//        }
	//        // write tile to list
	//        tpo.push_back(last);
	//        samecount = 0;
	//        last = tps[i];
	//      }
	//    }
	//    // catch the last tile
	//    if (samecount) {
	//      last.size.x = (1 + samecount) * tls.x;
	//      tpo.push_back(last);
	//    }
	//
	//    // No scan down Y, using different algo now that compressible blocks may
	//    // not be contiguous
	//    const auto xsave = tpo.size();
	//    samecount = 0;
	//    vector<tp> tpox;
	//    for (size_t i = 0; i < tpo.size(); ++i) {
	//      last = tpo[i];
	//      for (size_t j = i + 1; j < tpo.size(); ++j) {
	//        bool same = ((tpo[j].pos.x == last.pos.x) && (tpo[j].size == last.size) &&
	//                     (tpo[j].pos.y == last.pos.y + (tls.y * (1 + samecount))) &&
	//                     (tpo[j].rect == last.rect));
	//        if (same) {
	//          ++samecount;
	//          tpo.erase(tpo.begin() + j);
	//          --j;
	//        }
	//      }
	//      if (samecount) {
	//        last.size.y = (1 + samecount) * tls.y; // Expand tile
	//      }
	//      // write tile to list
	//      tpox.push_back(last);
	//      samecount = 0;
	//    }
	//
	//    tps.swap(tpox);
	//  }
	for (auto& t : tps) {
		auto s = make_unique<sf::RectangleShape>();
		s->setPosition(t.pos);
		s->setSize(t.size);
		s->setTexture(&_texture);
		s->setTextureRect(t.rect);
		
		_sprites.push_back(std::move(s));
	}
	
	cout << "Level with " << (_width * _height) << " Tiles, With " << nonempty
	<< " Not Empty, using: " << _sprites.size() << " Sprites\n";
}


void LevelSystem::render(RenderWindow& window) {
  for (auto& t : _sprites) {
    window.draw(*t);
  }
}

LevelSystem::TILES LevelSystem::getTile(sf::Vector2ul p) {//base 0. first row, col = 0 
  if (p.x >= helper->_json.columns_map || p.y >= helper->_json.rows_map) {
    throw string("Tile out of range: ") + to_string(p.x) + "," +
        to_string(p.y) + ")";
  }
  return _tiles[(p.y * helper->_json.columns_map) + p.x];
}

float LevelSystem::getWidth() { return helper->_json.columns_map * helper->_json.tileWidth_map; }

float LevelSystem::getHeight() { return helper->_json.rows_map * helper->_json.tileHeight_map; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
  return (Vector2f(p.x * helper->_json.tileWidth_map, p.y * helper->_json.tileHeight_map)) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::TILES type) {
  auto v = vector<sf::Vector2ul>();
  for (size_t i = 0; i < helper->_json.columns_map * helper->_json.rows_map; ++i) {
    if (_tiles[i] == type) {
      v.push_back({i % helper->_json.columns_map, i / helper->_json.columns_map });
    }
  }

  return v;
}

LevelSystem::TILES LevelSystem::getTileAt(Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    throw string("Tile out of range ");
  }
  return getTile(Vector2ul((v - _offset) / (helper->_json.tileWidth_map)));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
  auto a = v - _offset;
  if (a.x < 0 || a.y < 0) {
    return false;
  }
  auto p = Vector2ul((v - _offset) / (_tileSize));
  if (p.x > _width || p.y > _height) {
    return false;
  }
  return true;
}

void LevelSystem::setOffset(const Vector2f& _offset) {
  LevelSystem::_offset = _offset;
  buildSprites();
}

void LevelSystem::unload() {
  cout << "LevelSystem unloading\n";
  _sprites.clear();
  _tiles.reset();
  _width = 0;
  _height = 0;
  _offset = {0, 0};
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }


//JSON HELPER


json& jsonHelper::getJson(std::string path)
{
	string buffer;
	// Load in file to buffer
	ifstream f(path);
	if (f.good()) {
		f.seekg(0, std::ios::end);
		buffer.resize(f.tellg());
		f.seekg(0);
		f.read(&buffer[0], buffer.size());
		f.close();
	}
	else {
		throw string("Couldn't open level file: ") + path;
	}

	Document d;
	d.Parse(buffer.c_str());
	
	//might have to add check on getting values (or it will crash runtime if they are not found)
	_json.texture = d["tilesets"][0]["image"].GetString();
	_json.columns_tileset = d["tilesets"][0]["columns"].GetInt();
	_json.rows_tileset = d["tilesets"][0]["tilecount"].GetInt() / _json.columns_tileset;
	_json.tileHeight_tileset = d["tilesets"][0]["tileheight"].GetFloat();
	_json.tileWidth_tileset = d["tilesets"][0]["tilewidth"].GetFloat();
	_json.columns_map = d["layers"][0]["width"].GetInt();
	_json.rows_map = d["layers"][0]["height"].GetInt();
	_json.tileWidth_map = d["tilewidth"].GetFloat();
	_json.tileHeight_map = d["tileheight"].GetFloat();


	for (auto& item : d["layers"][0]["data"].GetArray()) {
		_json.data.push_back(item.GetInt());
	}


	return _json;
}



size_t jsonHelper::getGidAtPos(Vector2f pos) {
	Vector2ul pos2ul = Vector2ul((pos.x / _json.tileWidth_map), (pos.y / _json.tileHeight_map));
	return getGidAtPos(pos2ul);
}

size_t jsonHelper::getGidAtPos(sf::Vector2ul pos) {
	if (pos.x >= (_json.columns_map) || pos.y >= (_json.rows_map ) || pos.x < 0 || pos.y < 0) {
		throw string("Tile out of range: ") + to_string(pos.x) + "," +
			to_string(pos.y) + ")";
	}
	size_t pos_temp = pos.y * _json.columns_map + pos.x;
	return _json.data[pos_temp];
}

sf::IntRect jsonHelper::getIntRect(size_t gid)
{
	int actual_row_texture = (gid - 1) / _json.columns_tileset;
	int actual_col_texture = (gid - 1) % _json.columns_tileset;
	Vector2f pos_in_texture = sf::Vector2f(actual_col_texture * _json.tileWidth_map, actual_row_texture * _json.tileHeight_map);
	return IntRect(pos_in_texture.x, pos_in_texture.y, _json.tileWidth_tileset, _json.tileHeight_tileset);
}

std::shared_ptr<vector<ls::TILES>> jsonHelper::createTileList()
{
	shared_ptr<vector<ls::TILES>> _tiles = make_shared<vector<ls::TILES>>();
	vector<vector<int>> keys;
	
	//int c = 1;
	for (auto& gid : _json.data) {
		
			for (auto& item: limitsElementsTexture) {
				if (gid >= item.limits[0] && gid <= item.limits[1])
				{
					_tiles->push_back(item.types);
					break;
				}
				//used to check which gid was not set (had not a key which matches)
				/*else if (c == keys.size())
				{
					cout << gid << endl;
				}
				c++;*/
			}
			//c = 1;
		
	}

	if (_tiles->size() != _json.data.size())
		throw string("The TILES[] created size is different from the data read in the json file! Uncomment to discover which gid has not been set");

	return _tiles;
}

sf::Vector2f jsonHelper::getPosTileinMap(int pos)
{
	int actual_col_map = pos % _json.columns_map;
	int actual_row_map = pos / _json.columns_map;
	return sf::Vector2f(actual_col_map * _json.tileWidth_map, actual_row_map * _json.tileHeight_map);
}