#pragma once
#include "UIRect.h"

/*
    a dynamic grid of pixels, must be initalized with Create()
*/
class UIPixelGrid : public UIRect {

    public: 
        UIPixelGrid();

        void Draw(sf::RenderTarget&) override;

        void SetPixel(int x, int y, sf::Color colour);
        void Create(int width, int height, sf::Color fill_colour);

    private:
        sf::Texture texture;
        sf::Image image;
        
        bool created;
        bool changed;
};