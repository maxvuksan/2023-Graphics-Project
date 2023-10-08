#pragma once
#include "Component.h"

class SpriteLight : public Component {

    public:

        void Draw_ShadowPass(sf::RenderTarget& surface) override;

        void SetColour(sf::Color colour);
        void SetTexture(const char* texture);
    
    private:
        
        sf::Sprite light_sprite;
};