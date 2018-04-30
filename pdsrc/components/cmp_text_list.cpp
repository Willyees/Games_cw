#include "cmp_text_list.h"
#include "engine.h"
#include <SFML/Graphics/Rect.hpp>
#include <system_renderer.h>
#include <system_resources.h>
#include <windows.system.diagnostics.h>
#include <iostream>
using namespace std;
using namespace sf;

TextComponentList::TextComponentList(Entity * p, const std::vector<std::string> list)
	: Component(p), _text_list_index(0), changed(false){
	sf::Text text;
	for(std::string str : list) {
		text.setString(str);
		_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
		text.setFont(*_font);
		_text_list.push_back(text);
	}
	_text = _text_list.at(_text_list_index);
}

void TextComponentList::update(double dt)
{
	_text.setPosition(_parent->getPosition());
	//checking if the click was close and then change the text
	if (_text.getGlobalBounds().contains(_parent->scene->mouse_pos)) {
		if (!(_text_list_index++ < _text_list.size() - 1))
			_text_list_index = 0;
		_text = _text_list.at(_text_list_index);
		changed = true;
	}
	else
		changed = false;
}

void TextComponentList::render()
{
	Renderer::queue(&_text, _parent->isDynamic());
}

void TextComponentList::addText(std::string& str)
{
	sf::Text text; text.setString(str);
	_font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
	text.setFont(*_font);
	_text_list.push_back(text);
}

sf::Text TextComponentList::getText()
{
	return _text;
}

void TextComponentList::setDefault(std::string str)
{
	for (int i = 0; i < _text_list.size(); i++) {
		cout << (string)_text_list.at(i).getString() << endl;
		if (_text_list.at(i).getString() == (String) str) {
			_text_list_index = i;
			_text = _text_list.at(_text_list_index);
		}
	}
}
