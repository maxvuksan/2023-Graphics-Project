#include "PlayerWorldInteractions.h"
#include "../World/World.h"
#include "Inventory.h"

void PlayerWorldInteractions::Start(){
    
    cursor_graphic = object->GetScene()->AddUI<CursorGraphic>();
    selected_block = 0;
}

void PlayerWorldInteractions::LinkWorld(World* world){
    this->world = world;
}
void PlayerWorldInteractions::LinkInventory(Inventory* inventory){
    this->inventory = inventory;
}

void PlayerWorldInteractions::Update(){
    if(world == nullptr){
        std::cout << "ERROR: world variable is nullptr, please us PlayerWorldInteractions::LinkWorld(), PlayerWorldInteractions::Update()\n";
        return;
    }
    if(inventory == nullptr){
        std::cout << "ERROR : inventory variable is nullptr, please use PlayerWorldInteractions::LinkInventory(), void LinkInventory()\n";
        return;
    }

    CalculateMouse();
}

void PlayerWorldInteractions::CalculateMouse(){

    sf::Vector2i _mouse_pos = Mouse::DisplayPosition();
    sf::Vector2f player_center = object->GetScene()->GetActiveCamera()->WorldToScreenPosition(object->GetTransform()->position);

    _mouse_pos.x -= round(player_center.x);
    _mouse_pos.y -= round(player_center.y);

    mouse_world_pos = sf::Vector2i(round(object->GetTransform()->position.x) + _mouse_pos.x, 
                             round(object->GetTransform()->position.y) + _mouse_pos.y );



    sf::Vector2i cursor = world->RoundWorld(mouse_world_pos.x, mouse_world_pos.y);
    cursor_graphic->GetTransform()->position = sf::Vector2f(cursor.x, cursor.y);

    focused_block = world->GetTileWorld(cursor.x, cursor.y);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        if(cursor != focused_block_position){
            focused_block_position = cursor;

            breaking_completeness = 0;
            sound_increment = 0;
        }

        if(focused_block != -1){
            float increment = 0.02f * Time::Dt() / (float)Items[focused_block].durability;
            breaking_completeness += increment;
            sound_increment += increment;

            if(sound_increment > 0.3){
                sound_increment = 0;
                Sound::Play("hit", 30);
            }
        }

        cursor_graphic->SetComplete(breaking_completeness);

        if(breaking_completeness >= 1){
            // block breaks...
            Sound::Play("break", 10);
            world->BreakTileWorld(cursor.x, cursor.y, SetLocation::FOREGROUND, true);
            focused_block_position = sf::Vector2i(-1,-1); // clearing the focused block (an impossible position)
        }
    }
    else{
        cursor_graphic->SetComplete(0);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        
        int item_in_hand = inventory->GetItemInSelectedSlot();

        if(focused_block == -1 && item_in_hand != -1){
            
            if(world->SetTileWorld(item_in_hand, mouse_world_pos.x, mouse_world_pos.y, SetLocation::FOREGROUND, true)){
                inventory->DecrementSelectedSlot();
            }
        }
    }
}