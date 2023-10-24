#include "Minimap.h"

void Minimap::Start(){
    pixel_grid = AddComponent<UIPixelGrid>();
    pixel_grid->SetAlign(ScreenLocation::CENTER);
}

void Minimap::OnSetActive(){
    
    GetTransform()->scale = sf::Vector2f(1,1);
    pixel_grid->SetAlign(ScreenLocation::CENTER);
}

void Minimap::CatchEvent(sf::Event event){

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0) // moving up
        {
            GetTransform()->scale.x *= 1.1;
            GetTransform()->scale.y *= 1.1;
            pixel_grid->Align();
        }
        else if (event.mouseWheelScroll.delta < 0) // moving down
        {
            GetTransform()->scale.x *= 0.9;
            GetTransform()->scale.y *= 0.9;
            pixel_grid->Align();
        }
    }
}
