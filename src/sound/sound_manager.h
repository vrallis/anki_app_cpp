#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

/**
 * @class SoundManager
 * @brief Manages the loading and playing of sound effects.
 *
 * The SoundManager class provides functionality to load sound effects from files
 * and play them using their associated names.
 */
class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    bool loadSound(const std::string& path, const std::string& soundName);
    void playSound(const std::string& soundName);

private:
    std::map<std::string, Mix_Chunk*> sounds;
};

#endif // SOUND_MANAGER_H