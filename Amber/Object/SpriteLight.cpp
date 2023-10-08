#include "SpriteLight.h"
#include "../Core/AssetManager.h"


void SpriteLight::Draw_ShadowPass(sf::RenderTarget& surface){
    if(light_sprite.getTexture() == nullptr){
        return;
    }

    light_sprite.setPosition(
        object->GetScene()->GetActiveCamera()->WorldToScreenPosition(object->GetTransform()->position)
    );

    surface.draw(light_sprite, sf::BlendAlpha);
}

void SpriteLight::SetColour(sf::Color colour){
    light_sprite.setColor(colour);
}
void SpriteLight::SetTexture(const char* label){
    light_sprite.setTexture(*AssetManager::GetTexture(label));
}