#include "Mouse.h"
#include "../Core/Core.h"

sf::Vector2i Mouse::display_position(0,0);
sf::Vector2i Mouse::window_position(0,0);

void Mouse::UpdateDisplayPosition(){
    
    sf::Vector2i display_size = Core::GetDisplaySize();
    sf::Vector2i window_size = Core::GetWindowSize();

    display_position = sf::Vector2i(window_position.x * (display_size.x / (float)window_size.x), 
                                           window_position.y * display_size.y / (float)window_size.y);

}

void Mouse::SetWindowPosition(sf::Vector2i _window_position){
    window_position = _window_position;
}