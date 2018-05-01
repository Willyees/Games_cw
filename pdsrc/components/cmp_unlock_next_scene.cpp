#include "cmp_unlock_next_scene.h"
#include <engine.h>

using namespace std;
using namespace sf;

void UnlockNextSceneComponent::update(double dt)
{
	if(_parent->is_fordeletion()){
		_parent->scene->UnlockNextScene();
	}
		
	
}

UnlockNextSceneComponent::UnlockNextSceneComponent(Entity * p) 
	: Component(p)
{
}
