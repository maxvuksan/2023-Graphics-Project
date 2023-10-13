#include "Minimap.h"

void Minimap::Start(){
    pixel_grid = GetScene()->AddUI<UIPixelGrid>();
    pixel_grid->SetAlign(ScreenLocation::CENTER);
}

void Minimap::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        if(event.key.scancode == sf::Keyboard::Scan::M){
            pixel_grid->SetActive(!pixel_grid->IsActive());
            pixel_grid->SetAlign(ScreenLocation::CENTER);
        }
    }
}
