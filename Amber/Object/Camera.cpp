#include "Camera.h"
#include "Object.h"
#include "../Utility/Calc.h"
#include "../Core/Core.h"

sf::Vector2f Camera::bounded_position;

Camera::Camera() :background_colour(Globals::DEFAULT_BACKGROUND_COLOUR)
{}

sf::Vector2f Camera::WorldToScreenPosition(sf::Vector2f world){
    
    return world - bounded_position + sf::Vector2f(Core::GetDisplayWidth() / 2.0f, Core::GetDisplayHeight() / 2.0f);
}

sf::Vector2f Camera::ScreenToWorldPosition(sf::Vector2f screen){
    
    return screen + bounded_position - sf::Vector2f(Core::GetDisplayWidth() / 2.0f, Core::GetDisplayHeight() / 2.0f);
}
sf::Vector2f Camera::ScreenToWorldPosition(sf::Vector2i screen){
    
    return ScreenToWorldPosition(sf::Vector2f(screen.x, screen.y));
}

void Camera::Update(){
    Camera* active_cam = Scene::GetActiveCamera();

    bounded_position = active_cam->GetObject()->GetTransform()->position;

    bounded_position.x = Calc::Clamp(bounded_position.x, object->GetScene()->GetMinBounds().x + Core::GetDisplayWidth() / 2.0f , object->GetScene()->GetMaxBounds().x - Core::GetDisplayWidth() / 2.0f);
    bounded_position.y = Calc::Clamp(bounded_position.y, object->GetScene()->GetMinBounds().y + Core::GetDisplayHeight() / 2.0f, object->GetScene()->GetMaxBounds().y - Core::GetDisplayHeight() / 2.0f);
}