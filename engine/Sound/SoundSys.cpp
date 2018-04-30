#include "Sound.h"


SoundSys::SoundSys(std::string filename)
{
	if (filename.size() > 0) {
		if (filename.find(".ogg") != std::string::npos) {
			sbuffer *nbuf = new sbuffer;
			nbuf->name = filename;
                        nbuf->sbuf.loadFromFile(filename);
			ListOfSongs.push_back(*nbuf);
		}	
		else
		{
			throw("Sound File Error");
		}
	}
}

void SoundSys::SetMusicBuffer(std::string filename)
{
	if (filename.size() > 0) {
		if (filename.find(".ogg") != std::string::npos) {
			sbuffer *nbuf = new sbuffer;
			nbuf->name = filename;
                        nbuf->sbuf.loadFromFile(filename);
                        ListOfSongs.push_back(*nbuf);
		}
		else
		{
			throw("Sound File Error");
		}
	}
}
void SoundSys::SetSFXBuffer(std::string filename)
{
	if (filename.size() > 0) {
		if (filename.find(".ogg") != std::string::npos) {
			sbuffer *nbuf = new sbuffer;
			nbuf->name = filename;
                        nbuf->sbuf.loadFromFile(filename);
			ListOfSfx.push_back(*nbuf);
		}
		else
		{
			throw("Sound File Error");
		}
	}
}

sf::SoundBuffer *SoundSys::GetBuffer(std::string filename)
{
	for (short i = 0; i < ListOfSongs.size(); i++)
	{
		if (ListOfSongs.at(i).name == filename)
			return &ListOfSongs.at(i).sbuf;
	}
	return nullptr;
}


std::vector<sbuffer>* SoundSys::reSfx()
{
	return &ListOfSfx;
}

std::vector<sbuffer>* SoundSys::reSound()
{
	return &ListOfSongs;
}



