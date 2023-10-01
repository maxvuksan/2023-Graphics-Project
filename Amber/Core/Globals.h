#pragma once
#include <SFML/Graphics.hpp>

namespace Globals {

    /*
        Globals:

        Reusable global values which will be distributed by the core object. 

        accessed via Globals::var_name

    */
    const sf::Color DEFAULT_BACKGROUND_COLOUR = sf::Color(70,45,50);   
    const sf::Color BASE_SHADOW_COLOUR = sf::Color(150,130,130);

    const bool DEBUG_MODE = true;
    const sf::Color DEBUG_COLOUR = sf::Color(0,255,0);
    const sf::Color DEBUG_COLOUR_SECONDARY = sf::Color(255,255,0);
};


