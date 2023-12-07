#include "Camera.h"
#include "Object.h"
#include "../Utility/Calc.h"
#include "../Core/Core.h"

sf::Vector2f Camera::bounded_position;
sf::Vector2f Camera::ceil_bounded_position;

Camera::Camera() :background_colour(Globals::DEFAULT_BACKGROUND_COLOUR), ui_overlay_colour(sf::Color::Transparent)
{}

sf::Vector2f Camera::WorldToScreenPosition(sf::Vector2f world){
    
    return world - ceil_bounded_position + sf::Vector2f(Core::GetDisplayWidth() / 2.0f, Core::GetDisplayHeight() / 2.0f);
}

sf::Vector2f Camera::ScreenToWorldPosition(sf::Vector2f screen){
    
    return screen + ceil_bounded_position - sf::Vector2f(Core::GetDisplayWidth() / 2.0f, Core::GetDisplayHeight() / 2.0f);
}
sf::Vector2f Camera::ScreenToWorldPosition(sf::Vector2i screen){
    
    return ScreenToWorldPosition(sf::Vector2f(screen.x, screen.y));
}

void Camera::Update(){
    Camera* active_cam = Scene::GetActiveCamera();

    bounded_position = active_cam->GetThisObject()->GetTransform()->position;

    bounded_position.x = Calc::Clamp(bounded_position.x, object->GetScene()->GetMinBounds().x + Core::GetDisplayWidth() / 2.0f , object->GetScene()->GetMaxBounds().x - Core::GetDisplayWidth() / 2.0f);
    bounded_position.y = Calc::Clamp(bounded_position.y, object->GetScene()->GetMinBounds().y + Core::GetDisplayHeight() / 2.0f, object->GetScene()->GetMaxBounds().y - Core::GetDisplayHeight() / 2.0f);

    ceil_bounded_position = bounded_position;//sf::Vector2f(ceil(bounded_position.x), ceil(bounded_position.y));
}

void Camera::SetBackgroundTexture(const char* label){
    // assign texture to sprite
    this->background_sprite.setTexture(*AssetManager::GetTexture(label));
    // center texture
    this->background_sprite.setOrigin(sf::Vector2f(background_sprite.getTexture()->getSize().x / 2.0f, background_sprite.getTexture()->getSize().y / 2.0f));
}

sf::Sprite* Camera::GetBackgroundSprite(){
    
    // return nullptr if the no background texture has been assigned
    if(this->background_sprite.getTexture() == nullptr){
        return nullptr;
    }
    else{
        return &this->background_sprite;
    }
}