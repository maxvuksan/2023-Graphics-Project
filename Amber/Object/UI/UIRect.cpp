#include "UIRect.h"
#include "../Object.h"
#include "../../Core/Core.h"

int UIRect::padding = 8;

void UIRect::Align(){

    sf::Vector2i screen_size = Core::GetDisplaySize();

    float _width = object->GetTransform()->scale.x * width;
    float _height = object->GetTransform()->scale.y * height;

    sf::Vector2f positions[10] = {
        sf::Vector2f(padding,padding),
        sf::Vector2f(screen_size.x - _width - padding,padding),
        sf::Vector2f(screen_size.x / 2.0f - _width / 2.0f,padding),

        sf::Vector2f(padding,screen_size.y / 2.0f - _height / 2.0f),
        sf::Vector2f(screen_size.x - _width - padding,screen_size.y / 2.0f - _height / 2.0f ),
        sf::Vector2f(screen_size.x / 2.0f - _width / 2.0f,screen_size.y / 2.0f - _height / 2.0f),
        
        sf::Vector2f(padding, screen_size.y - _height - padding),
        sf::Vector2f(screen_size.x - _width - padding, screen_size.y - _height),
        sf::Vector2f(screen_size.x / 2.0f - _width / 2.0f, screen_size.y - _height - padding),

        object->GetTransform()->position, // FREE, does not realign
    };

    object->GetTransform()->position = positions[screen_location];
}

void UIRect::SetAlign(ScreenLocation _location){

    screen_location = _location;
    Align();
}
