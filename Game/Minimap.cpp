#include "Minimap.h"

void Minimap::Start(){
    pixel_grid = AddComponent<UIPixelGrid>();
    pixel_grid->SetAlign(ScreenLocation::CENTER);
    set_pan = false;
}

void Minimap::OnSetActive(){
    
    GetTransform()->scale = sf::Vector2f(1,1);
}

void Minimap::CatchEvent(sf::Event event){

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0) // moving up
        {
            GetTransform()->scale.x *= 1.1;
            GetTransform()->scale.y *= 1.1;
        }
        else if (event.mouseWheelScroll.delta < 0) // moving down
        {
            GetTransform()->scale.x *= 0.9;
            GetTransform()->scale.y *= 0.9;
        }
    }

    if(event.type == sf::Event::MouseButtonPressed){
        
        if(event.mouseButton.button == sf::Mouse::Button::Right){
            mouse_pan_inital = Mouse::DisplayPosition();
            pan_position_inital = GetTransform()->position;
            set_pan = true;
        }
    }

    if(event.type == sf::Event::MouseButtonReleased){
        
        if(event.mouseButton.button == sf::Mouse::Button::Right){
            set_pan = false;
            sf::Vector2i mouse_now = Mouse::DisplayPosition();
            GetTransform()->position = pan_position_inital + sf::Vector2f(mouse_now.x - mouse_pan_inital.x, mouse_now.y - mouse_pan_inital.y);
        }
    }
}

void Minimap::Update(){

    if(set_pan && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
    
        sf::Vector2i mouse_now = Mouse::DisplayPosition();
        GetTransform()->position = pan_position_inital + sf::Vector2f(mouse_now.x - mouse_pan_inital.x, mouse_now.y - mouse_pan_inital.y);
    }
}
