#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "system_renderer.h"

using namespace std;

MenuScene menu;
Level1Scene level1;
GameOverScene gameOver;
OptionScene optionScene;
//Level2Scene level2;
//Level3Scene level3;

int main() {

  Engine::Start(800, 600, "Potato's Destiny",&menu);


}
