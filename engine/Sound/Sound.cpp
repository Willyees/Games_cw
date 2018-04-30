#include "Sound.h"

static std::shared_ptr<Sound> tpl = nullptr;

std::shared_ptr<Sound> Factory::CreateSound()
{
	if (tpl == nullptr) {
		tpl = std::make_shared<Sound>();
	}

	return tpl;
}