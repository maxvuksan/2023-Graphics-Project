#include "Slot.h"
#include "../../Amber/Utility/Mouse.h"

int Slot::cellsize = 22; 
int Slot::spacing = 1; 

sf::Text Slot::item_count_text;
sf::Sprite Slot::item_sprite;
sf::RectangleShape Slot::slot_rectangle_shape;


void Slot::Construct(){
    

    slot_rectangle_shape.setSize(sf::Vector2f(cellsize, cellsize));
    slot_rectangle_shape.setFillColor(sf::Color::Transparent);
    slot_rectangle_shape.setOutlineThickness(1);


    sf::Texture* tiles_texture = AssetManager::GetTexture("tiles");
    sf::Font* font = AssetManager::GetFont("Amber_Default");

    if(tiles_texture == nullptr){
        std::cout << "ERROR : tiles_texture is nullptr, Slot::Construct\n";
        return;
    }
    if(font == nullptr){
        std::cout << "ERROR : font is nullptr, Slot::Construct()\n";
        return;
    }


    item_sprite.setTexture(*tiles_texture);
    item_count_text.setFont(*font);
    item_count_text.setOutlineColor(sf::Color(50,28, 49));
    item_count_text.setOutlineThickness(1);
    item_count_text.setCharacterSize(8);
}

bool Slot::Hovered(sf::Vector2f position){

    sf::Vector2i mouse_pos = Mouse::DisplayPosition();

    if(mouse_pos.x > position.x && mouse_pos.x < position.x + cellsize){     
        if(mouse_pos.y > position.y && mouse_pos.y < position.y + cellsize){
            
            return true;
        } 
    }
    
    return false;   
}

bool Slot::Occupied(){
    if(count > 0){
        return true;
    }
    return false;
}

void Slot::DrawSlot(sf::RenderTarget& surface, sf::Vector2f position){

    if(!Occupied()){
        return;
    }

    item_sprite.setPosition(position + sf::Vector2f(cellsize / 2.0f, cellsize / 2.0f));

    ItemDictionary::SetItemSprite(item_sprite, item_code);

    surface.draw(item_sprite);

    if(count > 1){
        item_count_text.setPosition(item_sprite.getPosition() + sf::Vector2f(0, -8));
        item_count_text.setString(std::to_string(count));
        surface.draw(item_count_text);
    }


}