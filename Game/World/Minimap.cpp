#include "Minimap.h"

void Minimap::Start(){
    pixel_grid = AddComponent<UIPixelGrid>();
    pixel_grid->SetAlign(ScreenLocation::CENTER);
    set_pan = false;
    pixel_grid->SetActive(false);
}

void Minimap::OnSetActive(){
    
    GetTransform()->scale = sf::Vector2f(1,1);
}

void Minimap::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        if(event.key.scancode == sf::Keyboard::Scan::M){
            
            pixel_grid->SetActive(!pixel_grid->IsActive());
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        // when scaling, the minimap is translated to ensure we are scaling around the mouse position

        float scale_factor = 0.1;

        // between mouse and minimap pos
        sf::Vector2f difference = sf::Vector2f(Mouse::DisplayPosition().x - GetTransform()->position.x, Mouse::DisplayPosition().y - GetTransform()->position.y);

        if (event.mouseWheelScroll.delta > 0) // moving up
        {
            GetTransform()->scale.x *= 1.0f + scale_factor;
            GetTransform()->scale.y *= 1.0f + scale_factor;
            difference *= 1.0f + scale_factor;
        }
        else if (event.mouseWheelScroll.delta < 0) // moving down
        {
            GetTransform()->scale.x *= 1.0f - scale_factor;
            GetTransform()->scale.y *= 1.0f - scale_factor;
            difference *= 1.0f - scale_factor;
        }

        GetTransform()->position = sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y) - difference;

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
