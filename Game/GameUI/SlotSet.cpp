#include "SlotSet.h"

sf::Sprite SlotSet::slot_sprite;

void SlotSet::Construct(){

    sf::Texture* texture = AssetManager::GetTexture("slot");
    if(texture == nullptr){
        std::cout << "texture returned by AssetManager is nullptr, SlotSet::Construct()\n";
        return;
    }
    slot_sprite.setTexture(*texture);
}

void SlotSet::Start(){

    ui_rect = AddComponent<UIRect>();
    ui_rect->SetAlign(ScreenLocation::FREE);
}

void SlotSet::SetAlign(ScreenLocation alignemnt){

    if(ui_rect == nullptr){
        std::cout << "UIRect component is nullptr in SlotSet::SetAlignNULL\n";
    }

    ui_rect->SetAlign(alignemnt);
}

void SlotSet::DefineGrid(int width, int height, SlotType type){
    
    slots.resize(width, {});

    for(int x = 0; x < width; x++){
        slots.push_back({});

        for(int y = 0; y < height; y++){
            slots.at(x).emplace_back(type);
        }
    }

    ui_rect->width = width * Slot::cellsize;
    ui_rect->height = height * Slot::cellsize;
}

void SlotSet::SetSlotType(int slot_x, int slot_y, SlotType type){
    slots.at(slot_x).at(slot_y).type = type;
}

void SlotSet::Draw(sf::RenderTarget& surface){
    
    hovered_slot.x = -1;
    hovered_slot.y = -1;
    
    // draw slot background
    for(int x = 0; x < slots.size(); x++){
        for(int y = 0; y < slots.at(x).size(); y++){

            sf::Vector2f position = GetTransform()->position + sf::Vector2f(x * Slot::cellsize, y * Slot::cellsize);
            slot_sprite.setTextureRect(sf::IntRect(Slot::cellsize * (int)slots[x][y].sprite, 0, Slot::cellsize, Slot::cellsize)); // getting appropriate sub sprite

            slot_sprite.setPosition(position);
            surface.draw(slot_sprite);

            if (slots[x][y].Hovered(position)) {
                hovered_slot.x = x;
                hovered_slot.y = y;
            }

        }
    } 

    // draw items
    for(int x = 0; x < slots.size(); x++){
        for(int y = 0; y < slots[x].size(); y++){

            sf::Vector2f position = GetTransform()->position + sf::Vector2f(x * Slot::cellsize, y * Slot::cellsize);
            slots[x][y].DrawSlot(surface, position);
        }
    } 
}

HoveredSlot SlotSet::GetHoveredSlotFromMultipleSets(std::vector<SlotSet*> slot_sets){
    
    for(int set = 0; set < slot_sets.size(); set++){

        for(int x = 0; x < slot_sets[set]->GetSlots()->size(); x++){
            for(int y = 0; y < slot_sets[set]->GetSlots()->at(x).size(); y++){

                sf::Vector2f position = slot_sets[set]->GetTransform()->position + sf::Vector2f(x * Slot::cellsize, y * Slot::cellsize);

                if (slot_sets[set]->GetSlots()->at(x)[y].Hovered(position)) {
                    return {slot_sets[set], x, y};
                }

            }
        } 
    }
    return {nullptr, -1, -1};
}
