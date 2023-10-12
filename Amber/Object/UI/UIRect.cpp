#include "UIRect.h"
#include "../../Core/Core.h"

void UIRect::Align(){

    sf::Vector2i screen_size = Core::GetDisplaySize();

    sf::Vector2f positions[10] = {
        sf::Vector2f(0,0),
        sf::Vector2f(screen_size.x - width,0),
        sf::Vector2f(screen_size.x / 2.0f - width / 2.0f,0),

        sf::Vector2f(0,screen_size.y / 2.0f - height / 2.0f),
        sf::Vector2f(screen_size.x - width,screen_size.y / 2.0f - height / 2.0f ),
        sf::Vector2f(screen_size.x / 2.0f - width / 2.0f,screen_size.y / 2.0f - height / 2.0f),
        
        sf::Vector2f(0, screen_size.y - height),
        sf::Vector2f(screen_size.x - width, screen_size.y - height),
        sf::Vector2f(screen_size.x / 2.0f - width / 2.0f, screen_size.y - height),

        GetTransform()->position, // FREE, does not realign
    };

    GetTransform()->position = positions[screen_location];
}

void UIRect::SetAlign(ScreenLocation _location){

    screen_location = _location;
    Align();
}
