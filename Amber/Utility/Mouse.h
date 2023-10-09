#pragma once
#include <SFML/Graphics.hpp>

/*
    window_position 
*/
class Mouse{

    public:

        static sf::Vector2i DisplayPosition(){
            return display_position;
        }

        static sf::Vector2i WindowPosition(){
            return window_position;
        }

        static void SetDisplayPosition(sf::Vector2i _display_position){
            display_position = _display_position;
        }
        static void SetWindowPosition(sf::Vector2i _window_position){
            window_position = _window_position;
        }
    private:

        static sf::Vector2i window_position;
        static sf::Vector2i display_position;


};