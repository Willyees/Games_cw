#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>


using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;
sf::View Renderer::view;
UserPreferences Engine::user_preferences;
bool Engine::_physicsPaused = false;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;


void Loading_update(float dt, const Scene* const scn) {
  //  cout << "Eng: Loading Screen\n";
  if (scn->isLoaded()) {
    cout << "Eng: Exiting Loading Screen\n";
    loading = false;
  } else {
    loadingspinner += 220.0f * dt;
    loadingTime += dt;
  }
}
void Loading_render() {
  // cout << "Eng: Loading Screen Render\n";
  static CircleShape octagon(80, 8);
  octagon.setOrigin(80, 80);
  octagon.setRotation(loadingspinner);
  octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
  octagon.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
  t.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f,0.3f));
  Renderer::queue(&t, false);//setting false (not dynamic) because loading text should not follow player movement
  Renderer::queue(&octagon, false);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
  static sf::Clock clock;
  float dt = clock.restart().asSeconds();
  {
    frametimes[++ftc] = dt;
    static string avg = _gameName + " FPS:";
    if (ftc % 60 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      _window->setTitle(avg + toStrDecPt(2, davg));
    }
  }

  if (loading) {
    Loading_update(dt, _activeScene);
  } else if (_activeScene != nullptr) {
	  if( !_physicsPaused )
	  {
		  Physics::update( dt );
	  }
    _activeScene->Update(dt);
  }
}

void Engine::Render(RenderWindow& window) {
  if (loading) {
    Loading_render();
  } else if (_activeScene != nullptr) {
    _activeScene->Render();
  }
  Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
                   const std::string& gameName, Scene* scn) {
  user_preferences.video_resolution_default = Vector2f(width, height);//store default resolution in order to use it when not on fullscreen
  RenderWindow window(VideoMode(width, height), gameName, user_preferences.fullscreen); //to make it full screen
  _gameName = gameName;
  _window = &window;
  Renderer::initialise(window);
  Physics::initialise();
  ChangeScene(scn);
  while (window.isOpen()) {
    Event event;
	_activeScene->mouse_pos = Vector2f(-1.0f, -1.0f);
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
		}
		if (event.type == Event::MouseButtonReleased) {
			_activeScene->mouse_pos = Vector2f(Mouse::getPosition(window));
		}
	}
    

    window.clear();
    Update();
    Render(window);
    window.display();
  }
  if (_activeScene != nullptr) {
    _activeScene->UnLoad();
    _activeScene = nullptr;
  }
  window.close();
  Physics::shutdown();
  Renderer::shutdown();
}

void Engine::setPhysicsPause( bool pause )
{
	_physicsPaused = pause;
}

std::shared_ptr<Entity> Scene::makeEntity(bool dynamic) {
  auto e = make_shared<Entity>(this, dynamic);
 
  ents.list.push_back(e);
  
  return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

Scene * Engine::getActiveScene()
{
	return _activeScene;
}

void Engine::ChangeScene(Scene* s) {
  cout << "Eng: changing scene: " << s << endl;

  // Force physics unpause when scene is changed, since new scene doesn't know if it is paused or not.
  _physicsPaused = false;
  
  auto old = _activeScene;
  _activeScene = s;

  if (old != nullptr) {
    old->UnLoad(); // todo: Unload Async
  }

  if (!s->isLoaded()) {
    cout << "Eng: Entering Loading Screen\n";
    loadingTime =0;
    _activeScene->Load();
    loading = true;
  }
}

void Scene::Update(const double& dt) { 
	ents.update(dt); 
	
}

void Scene::Render() { 
	
	ents.render(); 
}

void Scene::collisionHandler(Entity * entityA, Entity * entityB){}

void Scene::addScore(int score)
{
	scorePoints += score;
}

sf::View* Scene::getView()
{
	cout << &Renderer::view << " get view"<< endl;
	return &Renderer::view;
}

std::shared_ptr<Sprite> Scene::getBackground()
{
	return _background;
}

void Scene::setBackground(sf::Sprite& background)
{
	_background.reset();
	_background = make_shared<Sprite>(background);
}
void Scene::UnlockNextScene() { _nextSceneUnlocked = true; }
bool Scene::isLoaded() const {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    // Are we already loading asynchronously?
    if (_loaded_future.valid() // yes
        &&                     // Has it finished?
        _loaded_future.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
      // Yes
      _loaded_future.get();
      _loaded = true;
    }
    return _loaded;
  }
}
void Scene::setLoaded(bool b) {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    _loaded = b;
  }
}

void Scene::UnLoad() {
  ents.list.clear();
  
  setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
// Return time since Epoc
long long now() {
  return std::chrono::high_resolution_clock::now()
      .time_since_epoch()
      .count();
}
// Return time since last() was last called.
long long last() {
  auto n = now();
  static auto then = now();
  auto dt = n - then;
  then = n;
  return dt;
}
} // namespace timing

Scene::~Scene() { UnLoad(); }
Scene::Scene() : scorePoints(0), _loaded( false ) {}


