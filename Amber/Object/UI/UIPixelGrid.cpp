#include "UIPixelGrid.h"
#include <iostream>
UIPixelGrid::UIPixelGrid(): created(false), changed(false), blend_mode(sf::BlendNone){}

void UIPixelGrid::Draw(sf::RenderTarget& surface){
    if(!created){
        return;
    }

    RefreshTexture();

    sf::Sprite sprite;

    sprite.setScale(object->GetTransform()->scale);
    sprite.setPosition(object->GetTransform()->position);
    sprite.setTexture(texture, true);
    

    surface.draw(sprite);
}

void UIPixelGrid::SetPixel(int x, int y, sf::Color colour){
    image.setPixel(x, y, colour);
    changed = true;
}

sf::Color UIPixelGrid::GetPixel(int x, int y){

    return image.getPixel(x, y);
}

void UIPixelGrid::RefreshTexture(){
    if(changed){
        texture.loadFromImage(image);
        changed = false;
    }
}

void UIPixelGrid::Create(int _width, int _height, sf::Color fill_colour){

    image.create(_width, _height);
    for(int x = 0; x < _width; x++){
        for(int y = 0; y < _height; y++){
            image.setPixel(x, y, fill_colour);
        }
    }
    changed = true;
    created = true;

    this->width = _width;
    this->height = _height;
    Align();
}
