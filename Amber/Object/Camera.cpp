#include "Camera.h"
#include "Object.h"
#include "../Core/Core.h"

Camera::Camera() :background_colour(Globals::DEFAULT_BACKGROUND_COLOUR)
{}

sf::Vector2f Camera::WorldToScreenPosition(sf::Vector2f world){
    
    sf::Vector2i display_size = object->GetCore()->GetDisplaySize();
    
    return world + offset - object->GetTransform()->position + sf::Vector2f(display_size.x / 2.0f, display_size.y / 2.0f);
}