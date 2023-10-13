#include "SpriteRenderer.h"
#include "../Object.h"
#include "../../Core/Core.h"
#include <iostream>


void SpriteRenderer::Start(){}

void SpriteRenderer::Draw(sf::RenderTarget& surface){

    // convert position to camera relative position "screen position"
    sprite.setPosition(Camera::WorldToScreenPosition(
        object->GetTransform()->position
    ));
    
    surface.draw(sprite);
}

void SpriteRenderer::SetTexture(const char* label, bool center){
     
    sprite.setTexture(*AssetManager::GetTexture(label));
    // making the origin in the center of the sprite

    if(center){
        sprite.setOrigin(sprite.getTexture()->getSize().x / 2.0f, sprite.getTexture()->getSize().y / 2.0f);
    }
}

void SpriteRenderer::SetTextureRect(int left, int top, int width, int height){
    sprite.setTextureRect(sf::IntRect(left, top, width, height));
}