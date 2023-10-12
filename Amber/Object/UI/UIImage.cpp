#include "UIImage.h"
#include "../../Core/AssetManager.h"

void UIImage::Draw(sf::RenderTarget& surface){
    
    if(sprite.getTexture() == nullptr){
        std::cout << "ERROR : UIImage has no texture to draw, UIImage::Draw()\n";
        return;
    }

    sprite.setPosition(GetTransform()->position);
    surface.draw(sprite);
};

void UIImage::SetTexture(const char* label){
    this->texture = AssetManager::GetTexture(label);
    
    width = this->texture->getSize().x;
    height = this->texture->getSize().y;

    sprite.setTexture(*texture);
    Align();
}