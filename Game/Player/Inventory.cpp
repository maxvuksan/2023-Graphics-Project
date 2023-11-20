#include "Inventory.h"

int Inventory::row_count = 5;
int Inventory::row_length = 8;

void Inventory::Start(){

    InventorySlot::cellsize = 24;
    InventorySlot::spacing = 4;

    open = true;
    selected_slot = 0;

    // init inventory slots
    slots.resize(row_length);
    for(int x = 0; x < row_length; x++){
        
        slots[x].resize(row_count);

        for(int y = 0; y < row_count; y++){
            slots[x][y].SetPosition(sf::Vector2f((InventorySlot::spacing * 2 + (InventorySlot::cellsize + InventorySlot::spacing) * x), InventorySlot::spacing * 2 + (InventorySlot::cellsize + InventorySlot::spacing) * y));
        }
    }


    // init graphical elements
    rectangle_shape.setSize(sf::Vector2f(row_length * InventorySlot::cellsize + (row_length + 1) * InventorySlot::spacing, InventorySlot::cellsize + InventorySlot::spacing * 2));
    rectangle_shape.setFillColor(sf::Color(25, 28, 34, 130));
    rectangle_shape.setPosition(sf::Vector2f(InventorySlot::spacing,InventorySlot::spacing));

    rectangle_shape_inside.setFillColor(sf::Color(25, 28, 34, 130));
    rectangle_shape_inside.setPosition(sf::Vector2f(InventorySlot::spacing, InventorySlot::spacing * 3 + InventorySlot::cellsize));
    rectangle_shape_inside.setSize(sf::Vector2f(row_length * InventorySlot::cellsize + (row_length + 1) * InventorySlot::spacing, (row_count - 1) * InventorySlot::cellsize + (row_count - 1) * InventorySlot::spacing));

    InventorySlot::slot_rectangle_shape.setSize(sf::Vector2f(InventorySlot::cellsize, InventorySlot::cellsize));
    InventorySlot::slot_rectangle_shape.setFillColor(sf::Color::Transparent);
    InventorySlot::slot_rectangle_shape.setOutlineThickness(1);
    InventorySlot::slot_rectangle_shape.setOutlineColor(sf::Color::White);

    InventorySlot::item_sprite.setTexture(*AssetManager::GetTexture("tiles"));
    InventorySlot::item_sprite.setOrigin(sf::Vector2f(4,4));

    InventorySlot::item_count_text.setFont(*AssetManager::GetFont("Amber_Default"));
    InventorySlot::item_count_text.setCharacterSize(8);

    sf::Vector2u tex_size = InventorySlot::item_sprite.getTexture()->getSize();
    InventorySlot::items_per_row = round(tex_size.x / 8.0f); 
}

void Inventory::Draw(sf::RenderTarget& surface){
    
    surface.draw(rectangle_shape);


    // hotbar

    for(int x = 0; x < row_length; x++){
        
        //slot_rectangle_shape.setPosition(sf::Vector2f((spacing * 2 + (cellsize + spacing) * x), spacing * 2));
        
        if(selected_slot == x){
            InventorySlot::slot_rectangle_shape.setOutlineColor(sf::Color::Yellow);
        }
        else{
            InventorySlot::slot_rectangle_shape.setOutlineColor(sf::Color::White);
        }
        
        slots[x][0].RenderSlot(surface);
    }


    if(!open){
        return;
    }

    
    InventorySlot::slot_rectangle_shape.setOutlineColor(sf::Color::White);
    surface.draw(rectangle_shape_inside);

    // inside inventory slots
    for(int y = 1; y < row_count; y++){

        for(int x = 0; x < row_length; x++){
            //slot_rectangle_shape.setPosition(sf::Vector2f((spacing * 2 + (cellsize + spacing) * x), spacing * 2 + (cellsize + spacing) * y));
            
            slots[x][y].RenderSlot(surface);
        }

    }
}

void Inventory::CatchEvent(sf::Event event){
    
    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.scancode){

            case sf::Keyboard::Scan::E: 
                open = !open;
                break;
            case sf::Keyboard::Scan::Num1:
                selected_slot = 0;
                break;
            case sf::Keyboard::Scan::Num2:
                selected_slot = 1;
                break;
            case sf::Keyboard::Scan::Num3:
                selected_slot = 2;
                break;
            case sf::Keyboard::Scan::Num4:
                selected_slot = 3;
                break;
            case sf::Keyboard::Scan::Num5:
                selected_slot = 4;
                break;
            case sf::Keyboard::Scan::Num6:
                selected_slot = 5;
                break;
            case sf::Keyboard::Scan::Num7:
                selected_slot = 6;
                break;
            case sf::Keyboard::Scan::Num8:
                selected_slot = 7;
                break;
        }       
    }
}

void Inventory::PickupItem(ItemCode item){

    sf::Vector2i exisiting_stack = FindSlotContainingItem(item);
    // stack found
    if(exisiting_stack != sf::Vector2i(-1,-1)){

        AddItemToSlot(item, exisiting_stack.x, exisiting_stack.y);

        return;
    }
    // find free slot
    else{
        
        sf::Vector2i free_slot = FindNextFreeSlot();
        if(free_slot != sf::Vector2i(-1, -1)){
            AddItemToSlot(item, free_slot.x, free_slot.y);
        }
        
        return;
    }
    // full
    std::cout << "ignoring item, inventory full... Inventory::PickupItem()";
}


int Inventory::GetItemInSelectedSlot(){
    if(slots.at(selected_slot).at(0).occupied){
        return slots.at(selected_slot).at(0).item_code;
    }
    return -1;
}

void Inventory::DecrementSelectedSlot(){
    
    slots.at(selected_slot).at(0).count--;
    
    if(slots.at(selected_slot).at(0).count <= 0){
        slots.at(selected_slot).at(0).count = 0;
        slots.at(selected_slot).at(0).occupied = false;
    }
}


sf::Vector2i Inventory::FindNextFreeSlot(){
    for(int y = 0; y < row_count; y++){
        for(int x = 0; x < row_length; x++){

            if(!slots[x][y].occupied){
                return sf::Vector2i(x, y);
            }
        }
    }
    return sf::Vector2i(-1,-1);
}

sf::Vector2i Inventory::FindSlotContainingItem(ItemCode item){
    for(int y = 0; y < row_length; y++){
        for(int x = 0; x < row_count; x++){

            if(slots[x][y].item_code == item){
                return sf::Vector2i(x, y);
            }
        }
    }
    return sf::Vector2i(-1,-1);
}

void Inventory::AddItemToSlot(ItemCode item, int x, int y){
    if(slots[x][y].item_code == item){
        slots[x][y].count++;
        slots[x][y].occupied = true; 
    }
    else{
        slots[x][y].occupied = true;
        slots[x][y].item_code = item;
        slots[x][y].count = 1;
    }
}

