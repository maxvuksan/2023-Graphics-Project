#pragma once
#include "../Component.h"

class SpriteRenderer : public Component{
    
    public:

        SpriteRenderer();

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
        bool GetFlip();

        sf::Sprite* GetSprite(){return &sprite;}

        void SetOffset(sf::Vector2f offset);

    protected:
        sf::Vector2f offset;
        int flip_factor;
        sf::Sprite sprite;
        sf::Texture* texture;
};