#include "Minimap.h"

Minimap::Minimap(): created(false), changed(false), active(false){
    
}

void Minimap::Draw_Window(sf::RenderTarget& surface){
    if(!created || !active){
        return;
    }

    if(changed){
        texture.loadFromImage(image);
        changed = false;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    surface.draw(sprite);
};

void Minimap::CatchEvent(sf::Event event){
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.scancode == sf::Keyboard::Scan::M){
            active = !active;
        }
    
    }
       
}

void Minimap::SetPixel(int x, int y){
    image.setPixel(x, y, sf::Color::Black);
    changed = true;
}
void Minimap::RemovePixel(int x, int y){
    image.setPixel(x, y, sf::Color::White);
    changed = true;
}

void Minimap::Create(int width, int height){
    image.create(width, height);
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            image.setPixel(x, y, sf::Color::White);
        }
    }
    changed = true;
    created = true;
}