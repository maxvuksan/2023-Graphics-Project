#include "Camera.h"
#include "Object.h"
#include "../Core/Core.h"

Camera::Camera() :background_colour(Globals::DEFAULT_BACKGROUND_COLOUR)
{}

sf::Vector2f Camera::WorldToScreenPosition(sf::Vector2f world){
    
    Camera* active_cam = Scene::GetActiveCamera();
    sf::Vector2i display_size = Core::GetDisplaySize();
    
    return world + active_cam->offset - active_cam->GetObject()->GetTransform()->position + sf::Vector2f(display_size.x / 2.0f, display_size.y / 2.0f);
}