#include "UIImage.h"
#include "../../Core/AssetManager.h"
    

void UIImage::Draw(sf::RenderTarget& surface){

    // convert position to camera relative position "screen position"
    sprite.setPosition(Camera::WorldToScreenPosition(
        object->GetTransform()->position
    ));
    
    surface.draw(sprite);
}

void UIImage::SetTexture(const char* label, bool center){
     
    this->texture = AssetManager::GetTexture(label);
    sprite.setTexture(*this->texture);
    // making the origin in the center of the sprite

    if(center){
        sprite.setOrigin(sprite.getTexture()->getSize().x / 2.0f, sprite.getTexture()->getSize().y / 2.0f);
    }

    width = this->texture->getSize().x;
    height = this->texture->getSize().y;

    sprite.setTexture(*texture);
    Align();
}

sf::Texture* UIImage::GetTexture(){
    return this->texture;
}

void UIImage::SetTextureRect(int left, int top, int width, int height){
    sprite.setTextureRect(sf::IntRect(left, top, width, height));
}