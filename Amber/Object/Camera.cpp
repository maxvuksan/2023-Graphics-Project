#include "Camera.h"
#include "Object.h"
#include "../Utility/Calc.h"
#include "../Core/Core.h"

sf::Vector2f Camera::bounded_position;
sf::Vector2f Camera::ceil_bounded_position;

Camera::Camera() :background_colour(Globals::DEFAULT_BACKGROUND_COLOUR), ui_overlay_colour(sf::Color::Transparent), constant_screen_shake_factor(0), final_screen_wipe_colour(0,0,0,0), target_screen_wipe_colour(0,0,0,0), previous_screen_wipe_colour(0,0,0,0)
{}

sf::Vector2f Camera::WorldToScreenPosition(sf::Vector2f world){
    
    sf::Vector2f pos =  world - ceil_bounded_position + sf::Vector2f(Core::GetDisplayWidth() / 2.0f, Core::GetDisplayHeight() / 2.0f);
    return sf::Vector2f(floor(pos.x), floor(pos.y));
}


sf::Vector2f Camera::ScreenToWorldPosition(sf::Vector2f screen){
    
    return screen + ceil_bounded_position - sf::Vector2f(Core::GetDisplayWidth() / 2.0f, Core::GetDisplayHeight() / 2.0f);
}
sf::Vector2f Camera::ScreenToWorldPosition(sf::Vector2i screen){
    
    return ScreenToWorldPosition(sf::Vector2f(screen.x, screen.y));
}

void Camera::Update(){
    Camera* active_cam = Scene::GetActiveCamera();


    // calculate screen wipe colour (lerp overtime)

    if(milliseconds_to_screen_wipe_tracked < milliseconds_to_screen_wipe){

        milliseconds_to_screen_wipe_tracked += Time::Dt();
        milliseconds_to_screen_wipe_tracked = Calc::Clamp(milliseconds_to_screen_wipe_tracked, 0, milliseconds_to_screen_wipe);

        final_screen_wipe_colour = Calc::Lerp(previous_screen_wipe_colour, target_screen_wipe_colour, milliseconds_to_screen_wipe_tracked / milliseconds_to_screen_wipe);
    }


    // get random offset for screenshake
    float shake_x;
    float shake_y;

    if(constant_screen_shake_factor > 0){

        shake_x = rand() % (constant_screen_shake_factor * 2) - constant_screen_shake_factor;
        shake_y = rand() % (constant_screen_shake_factor * 2) - constant_screen_shake_factor;

    }

    bounded_position = active_cam->GetThisObject()->GetTransform()->position + sf::Vector2f(shake_x, shake_y);

    bounded_position.x = Calc::Clamp(bounded_position.x, object->GetScene()->GetMinBounds().x + Core::GetDisplayWidth() / 2.0f , object->GetScene()->GetMaxBounds().x - Core::GetDisplayWidth() / 2.0f);
    bounded_position.y = Calc::Clamp(bounded_position.y, object->GetScene()->GetMinBounds().y + Core::GetDisplayHeight() / 2.0f, object->GetScene()->GetMaxBounds().y - Core::GetDisplayHeight() / 2.0f);

    ceil_bounded_position = bounded_position;//sf::Vector2f(ceil(bounded_position.x), ceil(bounded_position.y));
}

void Camera::SetScreenWipeColourOvertime(sf::Color new_colour, int milliseconds){

    if(new_colour == target_screen_wipe_colour){
        return;
    }

    previous_screen_wipe_colour = final_screen_wipe_colour;
    target_screen_wipe_colour = new_colour;

    milliseconds_to_screen_wipe = milliseconds;
    milliseconds_to_screen_wipe_tracked = 0;
}

void Camera::SetScreenWipeColour(sf::Color new_colour){
    milliseconds_to_screen_wipe_tracked = 0;
    milliseconds_to_screen_wipe = 0;

    target_screen_wipe_colour = new_colour;
    final_screen_wipe_colour = new_colour;
}

void Camera::SetConstantScreenShake(int shake_factor){
    this->constant_screen_shake_factor = shake_factor;
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