#include "UtilityStation.h"

void UtilityStation::Start(){
    auto sr = AddComponent<SpriteRenderer>();

    slot_set = chunk->AddUIToChunk<SlotSet>();
    slot_set->DefineGrid(8,2, SlotType::OPEN);
    slot_set->SetActive(false);
}

void UtilityStation::LinkChunk(Chunk* chunk){
    this->chunk = chunk;
}

void UtilityStation::SetItemType(ItemCode item) {

    if(ItemDictionary::ITEM_DATA[item].type != ItemType::type_Utility){
        std::cout << "ERROR : Attempted to set a UtilityStation to a non utility item index, UtilityStation::SetItemType()\n";
        return;
    }

    utility_data = &ItemDictionary::UTILITY_BLOCK_DATA[ItemDictionary::ITEM_DATA[item].code_in_type];

    ItemDictionary::SetItemSprite(*this->GetComponent<SpriteRenderer>()->GetSprite(), item, false);
}

void UtilityStation::Update(){

    sf::Vector2f mouse = Camera::ScreenToWorldPosition(Mouse::DisplayPosition());
    
    hovered = false;

    if(GetTransform()->position.x + utility_data->footprint.x * ItemDictionary::tile_size > mouse.x && GetTransform()->position.x < mouse.x){
        if(GetTransform()->position.y + utility_data->footprint.y * ItemDictionary::tile_size > mouse.y && GetTransform()->position.y < mouse.y){
            hovered = true;
        }
    }
}

void UtilityStation::CatchEvent(sf::Event event){

    if (event.type == sf::Event::MouseButtonPressed) {
        if(event.mouseButton.button == sf::Mouse::Button::Right){
            
            if(hovered){
                OnOpen();
            }
        }
    }
    if(event.type == sf::Event::KeyPressed){
        if(event.key.scancode == sf::Keyboard::Scan::Escape){
            OnClose();
        }
    }
}


// @returns true if the bounds of this station overlaps the provided transforms
bool UtilityStation::FootprintOver(sf::Vector2i other_position, sf::Vector2i other_footprint){


    if(GetTransform()->position.y + utility_data->footprint.y * ItemDictionary::tile_size > other_position.y && GetTransform()->position.y < other_position.y + other_footprint.y * ItemDictionary::tile_size){

        if(GetTransform()->position.x + utility_data->footprint.x * ItemDictionary::tile_size > other_position.x && GetTransform()->position.x < other_position.x + other_footprint.x * ItemDictionary::tile_size){
            
            return true;
        }
    }
    return false;
}

bool UtilityStation::CanBreak(){

    switch(utility_data->breaking_behaviour){

        case UtilityBreakingBehaviour::ALWAYS: {
            return true;
        }

        case UtilityBreakingBehaviour::IF_EMPTY: {
            if(slot_set->)
        }
    }

    
}
