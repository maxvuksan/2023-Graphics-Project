#pragma once
#include "UIRect.h"

/*
    A static image, loaded from a texture in the asset manager
*/
class UIImage : public UIRect {

    public:
        void Draw(sf::RenderTarget&) override;
        void SetTexture(const char* label);

    private:
        sf::Texture* texture;
        sf::Sprite sprite;
};