#pragma once
#include <vector>
#include <C:/Users/Tomer/source/repos/Games_cw/lib/sfml/include/SFML/Audio.hpp>
#include <C:/Users/Tomer/source/repos/Games_cw/lib/sfml/include/SFML/Graphics.hpp>

typedef struct 
{
	sf::SoundBuffer sbuf;
	std::string name;
}sbuffer;

static class Sound
{
private:
	std::vector<sbuffer>ListOfSongs;
	std::vector<sbuffer>ListOfSfx;
public:
	Sound() {}
	Sound(std::string filename);

	void SetMusicBuffer(std::string filename);

	void SetSFXBuffer(std::string filename);

    sf::SoundBuffer *GetBuffer(std::string filename);

	std::vector<sbuffer>* reSfx();

	std::vector<sbuffer>* reSound();

	~Sound() {}
};
