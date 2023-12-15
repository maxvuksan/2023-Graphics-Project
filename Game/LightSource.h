#include "../Amber/Framework.h"
#pragma once

class LightSource : public Component { 
    
    public:

        void Start() override;

        ~LightSource() override;
    
        sf::Color colour = sf::Color(255,255,255);
        float decay = 0.07f;
        sf::Vector2f offset;
};