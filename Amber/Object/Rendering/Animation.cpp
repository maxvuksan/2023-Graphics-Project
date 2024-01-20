#include "Animation.h"
#include <iostream>
#include "../../Core/AssetManager.h"

Animation::Animation(const char* texture_label, int cellsize, int start, int end, int duration, bool loops):
    cellsize(cellsize), start(start), end(end), loops(true)
{
    texture = AssetManager::GetTexture(texture_label);
    durations.resize(end - start, duration);
}

Animation::Animation(const char* texture_label, int cellsize, int start, int end, std::vector<int> _durations, bool loops):
    texture(texture), cellsize(cellsize), start(start), end(end), loops(true)
{
    if(end - start != _durations.size()){
        std::cout << "ERROR : Trying to create an animation with a duration vector != frame count, Animation::Animation()\n";
        this->durations.resize(end - start, 20); // provide default value to prevent crash
        return;
    }
    
    // copy vector
    this->durations.resize(end - start);

    for(int i = 0; i < end - start; i++){
        this->durations[i] = _durations[i];
    }

    texture = AssetManager::GetTexture(texture_label);
}
