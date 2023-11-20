#include "InventorySlot.h"

// assigned my Inventory.cpp
int InventorySlot::cellsize; 

int InventorySlot::spacing; 
int InventorySlot::items_per_row; // in regards to the texture sheet 

sf::Text InventorySlot::item_count_text;
sf::Sprite InventorySlot::item_sprite;
sf::RectangleShape InventorySlot::slot_rectangle_shape;


void InventorySlot::SetPosition(sf::Vector2f position){
    this->position = position;
}

void InventorySlot::RenderSlot(sf::RenderTarget& surface){

    slot_rectangle_shape.setPosition(position);
    surface.draw(slot_rectangle_shape);

    if(!occupied){
        return;
    }

    // texture coords for item
    int tex_x = item_code % items_per_row; 
    int tex_y = floor(item_code / (float)items_per_row);

    item_sprite.setPosition(position + sf::Vector2f(cellsize / 2.0f, cellsize / 2.0f));
    item_sprite.setTextureRect(sf::IntRect(tex_x * 8, tex_y * 8, 8, 8));
    surface.draw(item_sprite);

    if(count > 1){
        item_count_text.setPosition(item_sprite.getPosition() + sf::Vector2f(0, -8));
        item_count_text.setString(std::to_string(count));
        surface.draw(item_count_text);
    }


}