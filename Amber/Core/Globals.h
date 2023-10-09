#pragma once
#include <SFML/Graphics.hpp>

/*
    Configurable global values 
    accessed via Globals::var_name

*/
namespace Globals {

    const sf::Color DEFAULT_BACKGROUND_COLOUR = sf::Color(63,45,70);   
    const sf::Color BASE_SHADOW_COLOUR = sf::Color(150,130,170);

    const bool DEBUG_MODE = false;
    const sf::Color DEBUG_COLOUR = sf::Color(0,255,0);
    const sf::Color DEBUG_COLOUR_SECONDARY = sf::Color(255,255,0);
    const sf::Color DEBUG_COLOUR_3 = sf::Color(255,100,100);
};


