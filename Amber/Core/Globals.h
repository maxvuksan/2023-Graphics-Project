#pragma once
#include <SFML/Graphics.hpp>




/*
    Configurable global values 
    accessed via Globals::var_name

*/
namespace Globals {

    const bool PROFILE_MEMORY = false;

    const int FRAMERATE_LIMIT = 300;

    const float PI = 3.1415927;
    const float HALF_PI = 1.5707963;
    const float QUARTER_PI = 0.7853981;

    const sf::Color DEFAULT_BACKGROUND_COLOUR = sf::Color(140, 150, 163);   
    const sf::Color BASE_SHADOW_COLOUR = sf::Color(51, 29, 35); //sf::Color(150/2,130/2,170/2);

    const sf::Color DEBUG_COLOUR = sf::Color(87, 235, 82);
    const sf::Color DEBUG_COLOUR_SECONDARY = sf::Color(255,255,0);
    const sf::Color DEBUG_COLOUR_3 = sf::Color(255,100,100);

    const sf::Vector2i HORIZONTAL_OFFSETS[4] = { sf::Vector2i(1,0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1)};
    
};


