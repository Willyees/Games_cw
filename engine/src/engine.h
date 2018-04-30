#pragma once
#include "user_preferences.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

class Scene {
public:
  Scene();
  virtual ~Scene();
  virtual void Load() = 0;
  virtual void LoadAsync();
  virtual void UnLoad();
  virtual void Update(const double& dt);
  virtual void Render();
  virtual void collisionHandler(Entity* entityA, Entity* entityB);
  void addScore(int score);
  sf::View* getView();
  std::shared_ptr<sf::Sprite> getBackground();
  void setBackground(sf::Sprite& background);
  bool isLoaded() const;
  std::shared_ptr<Entity> makeEntity(bool dynamic);
  
  EntityManager ents;
  sf::Vector2f mouse_pos;
protected:
	int scorePoints;
	std::shared_ptr<sf::Sprite> _background;
	void setLoaded(bool);
  
private:
  mutable bool _loaded;
  mutable std::future<void> _loaded_future;
  mutable std::mutex _loaded_mtx;
  bool _backgroundBool;
  
};

class Engine {
public:
  Engine() = delete;
  static void Start(unsigned int width, unsigned int height,
                    const std::string& gameName, Scene* scn);
  static void ChangeScene(Scene*);
  static sf::RenderWindow& GetWindow();
  static sf::Vector2u getWindowSize();
  static void setVsync(bool b);
  static Scene* getActiveScene();
  static UserPreferences user_preferences;
private:
  static Scene* _activeScene;
  static std::string _gameName;
  static void Update();
  static void Render(sf::RenderWindow& window);
};


namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing

