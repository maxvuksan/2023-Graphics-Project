#pragma once
#include <SFML/Graphics.hpp>

/*
    Configurable global values 
    accessed via Globals::var_name

*/
namespace Globals {

    const int FRAMERATE_LIMIT = 120;

    const sf::Color DEFAULT_BACKGROUND_COLOUR = sf::Color(63,45,70);   
    const sf::Color BASE_SHADOW_COLOUR = sf::Color(160,140,180);//sf::Color(150/2,130/2,170/2);

    const sf::Color DEBUG_COLOUR = sf::Color(0,255,0);
    const sf::Color DEBUG_COLOUR_SECONDARY = sf::Color(255,255,0);
    const sf::Color DEBUG_COLOUR_3 = sf::Color(255,100,100);
};


