#include "sound_manager.h"
#include <iostream>
#include <fstream>

SoundManager::SoundManager() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

SoundManager::~SoundManager() {
    for (auto& sound : sounds) {
        Mix_FreeChunk(sound.second);
    }
    sounds.clear();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

bool SoundManager::loadSound(const std::string& path, const std::string& soundName) {
    // Check if the file exists
    std::ifstream file(path);
    if (!file.good()) {
        std::cerr << "File not found: " << path << std::endl;
        return false;
    }

    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    sounds[soundName] = sound;
    return true;
}

void SoundManager::playSound(const std::string& soundName) {
    if (sounds.find(soundName) != sounds.end()) {
        Mix_PlayChannel(-1, sounds[soundName], 0);
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}