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

void SlotSet::Align(ScreenLocationX align_x, ScreenLocationY align_y){

    UIRect::Align(GetTransform(), GetWidth(), GetHeight(), align_x, align_y);
}

void SlotSet::DefineGrid(int width, int height, SlotType type){

    for(int i = 0; i < slots.size(); i++){
        slots.at(i).clear();
    }
    slots.clear();

    for(int x = 0; x < width; x++){
        slots.push_back({});
        for(int y = 0; y < height; y++){
            slots.at(x).emplace_back(type);
        }
    }
}

void SlotSet::DefineRecipeGrid(const std::vector<Recipes>& _recipes){

    if(_recipes.size() != slots.size() * slots[0].size()){
        std::cout << "ERROR : recipe vector does not match the slots vector, SlotSet::DefineRecipeGrid\n";
        return;
    }

    recipes.clear();
    recipes.resize(slots.size(), {});

    for(int x = 0; x < slots.size(); x++){
        for(int y = 0; y < slots.at(0).size(); y++){
            
            int index = x + y * slots.size();

            recipes.at(x).push_back(_recipes.at(index));

            slots.at(x).at(y).type = SlotType::RECIPE;
            slots.at(x).at(y).sprite = SlotSprite::RECIPE;

            slots.at(x).at(y).item_code = ItemDictionary::RECIPE_DATA[_recipes.at(index)].result.item_code;
            slots.at(x).at(y).count = ItemDictionary::RECIPE_DATA[_recipes.at(index)].result.count;
        }
    }
}

int SlotSet::GetWidth(){
    return slots.size() * Slot::cellsize;
}

int SlotSet::GetHeight(){
    return slots.at(0).size() * Slot::cellsize;
}

int SlotSet::GetRowLength(){
    return slots.size();
}
int SlotSet::GetRowCount(){
    return slots.at(0).size();
}


void SlotSet::SetSlotType(int slot_x, int slot_y, SlotType type){
    slots.at(slot_x).at(slot_y).type = type;
}

void SlotSet::Draw(sf::RenderTarget& surface){
    
    // draw slot background
    for(int x = 0; x < slots.size(); x++){
        for(int y = 0; y < slots.at(x).size(); y++){

            sf::Vector2f position = GetTransform()->position + sf::Vector2f(x * Slot::cellsize, y * Slot::cellsize);
            slot_sprite.setTextureRect(sf::IntRect(Slot::cellsize * (int)slots[x][y].sprite, 0, Slot::cellsize, Slot::cellsize)); // getting appropriate sub sprite

            slot_sprite.setPosition(position);
            surface.draw(slot_sprite);

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

bool SlotSet::IsEmpty(){

    for(int x = 0; x < slots.size(); x++){
        for(int y = 0; y < slots.at(x).size(); y++){

            if(slots[x][y].count > 0){
                return false;
            }

        }
    } 
    return true;
}

HoveredSlot SlotSet::GetHoveredSlotFromMultipleSets(std::vector<SlotSet*>& slot_sets){
    
    // iterates over each provided slot set, returning when a slot is hovered by the mouse
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

const RecipeData& SlotSet::GetRecipe(int x, int y){
    return ItemDictionary::RECIPE_DATA[recipes.at(x).at(y)];
}