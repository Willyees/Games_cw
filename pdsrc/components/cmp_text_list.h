#pragma once
#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextComponentList : public Component {
public:
	bool changed;
	TextComponentList() = delete;
	explicit TextComponentList(Entity* p, const std::vector<std::string> str);
	
	void update(double dt) override;

	void render() override;

	~TextComponentList() override = default;
	void addText(std::string& str);
	sf::Text getText();
	void setDefault(std::string str);
protected:
	std::shared_ptr<sf::Font> _font;
	std::string _string;
	sf::Text _text;
	std::vector<sf::Text> _text_list;
private:
	unsigned int _text_list_index;
};
