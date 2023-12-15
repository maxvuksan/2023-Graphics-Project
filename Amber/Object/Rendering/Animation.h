#pragma once
#include <SFML/Graphics.hpp>

/*
    Configuration for an animation
    @param texture_label the texture we are referencing for the animation
    @param cellsize the horizontal distance between frames (note spritesheets must be oriented horizontally, it is assumed the y position never changes)
    @param start the starting frame, position cellsize * start
    @param end the ending frame, position cellsize * end
    @param duration how long is each frame, provided as a single value, or vector, where vector[i] is duration of frame[i]
*/
class Animation {

    public:
        /*
            @param texture_label the texture we are referencing for the animation
            @param cellsize the horizontal distance between frames (note spritesheets must be oriented horizontally, it is assumed the y position never changes)
            @param start the starting frame, position cellsize * start
            @param end the ending frame, position cellsize * end
            @param duration how long is each frame, provided as a single value, or vector, where vector[i] is duration of frame[i]
        */
        Animation(const char* texture_label, int cellsize, int start, int end, int duration, bool loops = true);
        Animation(const char* texture_label, int cellsize, int start, int end, std::vector<int> durations, bool loops = true);

        sf::Texture* GetTexture() const { return texture;}
        int GetCellsize() const { return cellsize;}
        int GetStart() const { return start;}
        int GetEnd() const { return end;}
        bool GetLoops() const {return loops;}
        std::vector<int>* GetDurations() {return &durations; }

    private:
        sf::Texture* texture;
        int cellsize;
        int start;
        int end;
        bool loops;
        std::vector<int> durations;
};