#pragma once
#include <SDL_mixer.h>
#include <string>
#include "Main.h"
using namespace std;
class MixSoundEffect
{
public:
	MixSoundEffect() = default;
	
	MixSoundEffect(Mix_Chunk* sound) {
		soundFX = sound;
	}

	MixSoundEffect(const MixSoundEffect& sound) {
		soundFX = sound.soundFX;
	}

	MixSoundEffect& operator=(const MixSoundEffect& music) {
		this->soundFX = music.soundFX;
	}

	void LoadMusic(string path) {
		soundFX = Mix_LoadWAV(path.c_str());
		if (soundFX == nullptr) {
			LogMUSErrorMsg("Failed to load music");
		}
	}

	~MixSoundEffect() {
		Mix_FreeChunk(soundFX);
		soundFX = nullptr;
	}
private:
	Mix_Chunk* soundFX;
};

