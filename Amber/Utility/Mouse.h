#pragma once
#include <SFML/Graphics.hpp>

/*
    Provides access to the mouse position,
    
    relative to the window ::WindowPosition()
    or relative to the display ::DisplayPosition()
*/ 
class Mouse{

    public:

        static sf::Vector2i DisplayPosition(){
            return display_position;
        }

        static sf::Vector2i WindowPosition(){
            return window_position;
        }

        // calculates the display_position from the window_position
        static void UpdateDisplayPosition();
        static void SetWindowPosition(sf::Vector2i _window_position);
    private:

        static sf::Vector2i window_position;
        static sf::Vector2i display_position;


};