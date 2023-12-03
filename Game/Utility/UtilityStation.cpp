#include "UtilityStation.h"

void UtilityStation::Start(){
    auto sr = AddComponent<SpriteRenderer>();
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


// @returns true if the footprint over laps this stations footprint
bool UtilityStation::FootprintOver(sf::Vector2i position, sf::Vector2i footprint){

    if(GetTransform()->position.y + utility_data->footprint.y > position.y && GetTransform()->position.y < position.y + footprint.y){
        if(GetTransform()->position.x + utility_data->footprint.x > position.y && GetTransform()->position.x < position.x + footprint.x){
            return true;
        }
    }

    return false;
}
