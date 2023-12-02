#pragma once
#include <SFML/Audio.hpp>

class Sound {

    public:
        static void Construct();
        static void Play(const char* label, float volume = 15, float pitch = 1);

    private:
        static std::vector<sf::Sound> sound_cycle;
        static int sound_index;
        static const int sound_limit;
}; 