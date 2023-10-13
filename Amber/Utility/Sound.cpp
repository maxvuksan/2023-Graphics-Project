#include "Sound.h"
#include "../Core/AssetManager.h"

const int Sound::sound_limit = 15;
int Sound::sound_index = 0;

std::vector<sf::Sound> Sound::sound_cycle;

void Sound::Construct(){
    sound_cycle.clear();
    sound_cycle.resize(sound_limit, sf::Sound());
    sound_index = 0;
}

void Sound::Play(const char* label, float volume, float pitch){
    
    sf::SoundBuffer* sound_buf = AssetManager::GetSound(label);
    
    sound_cycle[sound_index].setVolume(volume);
    sound_cycle[sound_index].setPitch(pitch);
    sound_cycle[sound_index].setBuffer(*sound_buf);
    sound_cycle[sound_index].play();

    sound_index++;
    sound_index %= sound_limit;
}