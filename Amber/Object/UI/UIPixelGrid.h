#pragma once
#include "UIRect.h"

/*
    a dynamic grid of pixels, must be initalized with Create().

    uses an underyling image to store colour data in a grid fashion.
*/
class UIPixelGrid : public UIRect {

    public: 
        UIPixelGrid();

        void Draw(sf::RenderTarget&) override;

        /*
            @returns the colour of the pixel in the underlying image
            @param x coordinate to fetch from
            @param y coordinate to fetch from
        */
        sf::Color GetPixel(int x, int y);

        /*
            sets a pixel on the underyling image
            @param x coordinate of the pixel
            @param y coordinate of the pixel
            @param colour to change the pixel to
        */
        void SetPixel(int x, int y, sf::Color colour);


        /* 
            calls the create method on the underlying image
            @param width of the image
            @param height of the image
            @param fill_colour the default colour to fill the image
            
        */
        void Create(int width, int height, sf::Color fill_colour);

        // refreshes the texture, loading data from the image if texture has changed
        void RefreshTexture();
        // @returns a texture holding the current state of image
        sf::Texture& GetTexture(){return texture;}
        sf::Image& GetImage(){return image;}


    private:
        sf::Texture texture;
        sf::Image image;
        sf::BlendMode blend_mode;

        bool created;
        bool changed;
};