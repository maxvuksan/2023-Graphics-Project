#pragma once
#include "UIRect.h"
#include "../Rendering/SpriteRenderer.h"
/*
    A static image, loaded from a texture in the asset manager
*/
class UIImage : public UIRect {

    public:
        void Draw(sf::RenderTarget& surface) override;

        void SetTexture(const char* label, bool center = false);
        void SetTextureRect(int left, int top, int width, int height);
        sf::Texture* GetTexture();

    protected:
        sf::Sprite sprite;
        sf::Texture* texture;
};