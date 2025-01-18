#include "sound_manager.h"
#include <iostream>
#include <fstream>

/**
 * @brief Constructs a new SoundManager object and initializes the SDL audio subsystem.
 * 
 * This constructor initializes the SDL audio subsystem and the SDL_mixer library.
 * If SDL or SDL_mixer fails to initialize, an error message is printed to the standard error stream.
 * 
 * @note SDL_Init(SDL_INIT_AUDIO) initializes the SDL audio subsystem.
 * @note Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) initializes the SDL_mixer library with a frequency of 44100 Hz, 
 *       the default format, 2 channels (stereo), and a chunk size of 2048 bytes.
 * 
 * @see SDL_Init
 * @see Mix_OpenAudio
 * @see SDL_GetError
 * @see Mix_GetError
 */
SoundManager::SoundManager() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

/**
 * @brief Destructor for the SoundManager class.
 *
 * This destructor is responsible for cleaning up all the resources
 * associated with the SoundManager. It iterates through the stored
 * sounds, frees each sound chunk, clears the sounds container, and
 * shuts down the audio and SDL subsystems.
 */
SoundManager::~SoundManager() {
    for (auto& sound : sounds) {
        Mix_FreeChunk(sound.second);
    }
    sounds.clear();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

/**
 * @brief Loads a sound file and stores it in the sound manager.
 *
 * This function attempts to load a sound file from the specified path and associates it with a given sound name.
 * If the file does not exist or fails to load, an error message is printed to the standard error stream.
 *
 * @param path The file path to the sound file to be loaded.
 * @param soundName The name to associate with the loaded sound.
 * @return true if the sound file was successfully loaded and stored, false otherwise.
 */
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

/**
 * @brief Plays the specified sound if it exists in the sound manager.
 *
 * This function checks if the sound with the given name exists in the sound manager's
 * collection of sounds. If the sound is found, it plays the sound on any available
 * channel. If the sound is not found, it outputs an error message to the standard error stream.
 *
 * @param soundName The name of the sound to be played.
 */
void SoundManager::playSound(const std::string& soundName) {
    if (sounds.find(soundName) != sounds.end()) {
        Mix_PlayChannel(-1, sounds[soundName], 0);
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}