#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "system_renderer.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
GameOverScene gameOver;
//Level2Scene level2;
//Level3Scene level3;

int main() {
  Engine::Start(1280, 800, "Potato's Destiny",&menu);

}
