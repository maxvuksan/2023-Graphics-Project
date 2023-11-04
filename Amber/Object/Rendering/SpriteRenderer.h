#pragma once
#include "../Component.h"

class SpriteRenderer : public Component{
    
    public:

        void Start() override;
        void Draw(sf::RenderTarget& surface) override;

        /*
            @param label Name of the texture
        */
        void SetTexture(const char* label, bool center = false);
        void SetTextureRect(int left, int top, int width, int height);
        sf::Texture* GetTexture();
        // should the sprite be flipped?
        void SetFlip(bool flip);

    protected:
        int flip_val;
        sf::Sprite sprite;
        sf::Texture* texture;
};