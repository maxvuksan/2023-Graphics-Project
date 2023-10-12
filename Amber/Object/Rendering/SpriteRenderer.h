#pragma once
#include "../Component.h"

class SpriteRenderer : public Component{
    
    public:

        void Start() override;
        void Draw(sf::RenderTarget& surface) override;

        /*
            @param label Name of the texture
        */
        void SetTexture(const char* label);

    private:
        sf::Sprite sprite;
};