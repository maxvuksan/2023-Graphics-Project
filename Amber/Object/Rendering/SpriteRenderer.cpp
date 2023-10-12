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
    sprite.setScale(object->GetTransform()->scale);
    sprite.setRotation(object->GetTransform()->rotation);
    
    surface.draw(sprite);
}

void SpriteRenderer::SetTexture(const char* label){
     
    sprite.setTexture(*AssetManager::GetTexture(label));
    // making the origin in the center of the sprite
    sprite.setOrigin(sprite.getTexture()->getSize().x / 2.0f, sprite.getTexture()->getSize().y / 2.0f);
}