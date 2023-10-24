#include "UIRect.h"
#include "../Object.h"
#include "../../Core/Core.h"

void UIRect::Align(){

    sf::Vector2i screen_size = Core::GetDisplaySize();

    float _width = object->GetTransform()->scale.x * width;
    float _height = object->GetTransform()->scale.y * height;

    sf::Vector2f positions[10] = {
        sf::Vector2f(0,0),
        sf::Vector2f(screen_size.x - _width,0),
        sf::Vector2f(screen_size.x / 2.0f - _width / 2.0f,0),

        sf::Vector2f(0,screen_size.y / 2.0f - _height / 2.0f),
        sf::Vector2f(screen_size.x - _width,screen_size.y / 2.0f - _height / 2.0f ),
        sf::Vector2f(screen_size.x / 2.0f - _width / 2.0f,screen_size.y / 2.0f - _height / 2.0f),
        
        sf::Vector2f(0, screen_size.y - _height),
        sf::Vector2f(screen_size.x - _width, screen_size.y - _height),
        sf::Vector2f(screen_size.x / 2.0f - _width / 2.0f, screen_size.y - _height),

        object->GetTransform()->position, // FREE, does not realign
    };

    object->GetTransform()->position = positions[screen_location];
}

void UIRect::SetAlign(ScreenLocation _location){

    screen_location = _location;
    Align();
}
