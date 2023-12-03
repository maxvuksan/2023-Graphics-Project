#include "PlayerWorldInteractions.h"
#include "../World/World.h"
#include "Inventory.h"
#include "../Utility/UtilityStation.h"
#include "../Utility/Chest.h"

void PlayerWorldInteractions::Start(){
    
    cursor_graphic = object->GetScene()->AddUI<CursorGraphic>();
    selected_block = 0;
    auto_target_blocks = false;
}

void PlayerWorldInteractions::LinkWorld(World* world){
    this->world = world;
}
void PlayerWorldInteractions::LinkInventory(Inventory* inventory){
    this->inventory = inventory;
}

void PlayerWorldInteractions::Draw(sf::RenderTarget& surface){
    if(world == nullptr){
        std::cout << "ERROR: world variable is nullptr, please us PlayerWorldInteractions::LinkWorld(), PlayerWorldInteractions::Update()\n";
        return;
    }
    if(inventory == nullptr){
        std::cout << "ERROR : inventory variable is nullptr, please use PlayerWorldInteractions::LinkInventory(), void LinkInventory()\n";
        return;
    }

    CalculateMouse(surface);
}

void PlayerWorldInteractions::CatchEvent(sf::Event event){
    
    if (event.type == sf::Event::KeyPressed)
    {
        if(event.key.scancode == sf::Keyboard::Scan::LAlt){
            
            auto_target_blocks = !auto_target_blocks;
            cursor_graphic->SetAutoTargetBlocks(auto_target_blocks);
        }
    }
}

void PlayerWorldInteractions::CalculateMouse(sf::RenderTarget& surface){

    cursor_graphic->SetActive(true);

    sf::Vector2i _mouse_pos = Mouse::DisplayPosition();
    sf::Vector2f player_center = object->GetScene()->GetActiveCamera()->WorldToScreenPosition(object->GetTransform()->position);

    _mouse_pos.x -= round(player_center.x);
    _mouse_pos.y -= round(player_center.y);

    mouse_world_pos = sf::Vector2i(round(object->GetTransform()->position.x) + _mouse_pos.x, 
                             round(object->GetTransform()->position.y) + _mouse_pos.y );


    sf::Vector2i world_tile = mouse_world_pos;
    sf::Vector2i coord_tile = world->WorldToCoord(mouse_world_pos.x, mouse_world_pos.y);

    if(auto_target_blocks){

        sf::Vector2i auto_tile = world->GetNearestTileInDirectionOfMouse(object->GetTransform()->position, SetLocation::MAIN);
        
        if(auto_tile != sf::Vector2i(-1,-1)){
            coord_tile = auto_tile;
            // convert auto coord to world space
            world_tile = world->CoordToWorld(coord_tile.x, coord_tile.y);
        }
    }

    sf::Vector2i rounded_world = world->RoundWorld(world_tile.x, world_tile.y);


    int item_in_hand = inventory->GetItemInSelectedSlot();
    const ItemData* data = &ItemDictionary::ITEM_DATA[item_in_hand];
    
    
    float picaxe_speed = 1;

    if(data->type == ItemType::type_Picaxe){
        picaxe_speed = ItemDictionary::PICAXE_DATA[data->code_in_type].speed;
    }

    cursor_graphic->GetTransform()->position = sf::Vector2f(rounded_world.x, rounded_world.y);
    focused_block = world->GetTile(coord_tile.x, coord_tile.y);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        if(world_tile != focused_block_position){
            focused_block_position = world_tile;

            breaking_completeness = 0;
            sound_increment = 0;
        }

        if(focused_block != -1){
            float increment = 0.004f * Time::Dt() / (float)ItemDictionary::MAIN_BLOCK_DATA[focused_block].durability * picaxe_speed;
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
            world->BreakTileWorld(world_tile.x, world_tile.y, SetLocation::MAIN, true);
            focused_block_position = sf::Vector2i(-1,-1); // clearing the focused block (an impossible position)
        }
    }
    else{
        cursor_graphic->SetComplete(0);
    }

        

    if(item_in_hand == -1){
        return;
    }
    
    // for utility objects, check if footprint overlaps a tile
    if(data->type == ItemType::type_Utility){
        
        cursor_graphic->SetActive(false);
        utility_location_valid = true;

        for(int foot_x = 0; foot_x < ItemDictionary::UTILITY_BLOCK_DATA[data->code_in_type].footprint.x; foot_x++){

            // ensuring we are placing on ground
            if(world->GetTile(foot_x + coord_tile.x, coord_tile.y + ItemDictionary::UTILITY_BLOCK_DATA[data->code_in_type].footprint.y, SetLocation::MAIN) == -1){
                utility_location_valid = false;
            }

            for(int foot_y = 0; foot_y < ItemDictionary::UTILITY_BLOCK_DATA[data->code_in_type].footprint.y; foot_y++){
                
                if(world->GetTile(foot_x + coord_tile.x, foot_y + coord_tile.y, SetLocation::MAIN) != -1){
                    utility_location_valid = false;
                }
            }
        }

        ItemDictionary::SetItemSprite(utility_hologram, (ItemCode)item_in_hand, false);
        
        if(utility_location_valid){
            utility_hologram.setColor(sf::Color(255,255,255,180));
        }
        else{
            utility_hologram.setColor(sf::Color(255,90,90,180));
        }
        utility_hologram.setPosition(Camera::WorldToScreenPosition(sf::Vector2f(rounded_world.x, rounded_world.y)));

        surface.draw(utility_hologram);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        
        if(focused_block != -1){
            return;
        }


        if(world->CoordIsConnectedToOtherTiles(coord_tile.x, coord_tile.y)){

            if(data->type == ItemType::type_Main){

                if(world->SetTile(data->code_in_type, coord_tile.x, coord_tile.y, SetLocation::MAIN, SetMode::OVERRIDE, true)){
                    inventory->DecrementSelectedSlot();
                }
            }
            if(data->type == ItemType::type_Foreground){

                if(world->SetTile(data->code_in_type, coord_tile.x, coord_tile.y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true)){
                    inventory->DecrementSelectedSlot();
                }

            }
            if(data->type == ItemType::type_Background){
                
                if(world->SetTile(data->code_in_type, coord_tile.x, coord_tile.y, SetLocation::BACKGROUND,SetMode::OVERRIDE, true)){
                    inventory->DecrementSelectedSlot();
                }
            }
        }


        if(data->type == ItemType::type_Utility){

            if(utility_location_valid){
                
                PlaceUtility(rounded_world, coord_tile, (ItemCode)item_in_hand);
            }
        }

    }

}

void PlayerWorldInteractions::PlaceUtility(const sf::Vector2i& rounded_world, const sf::Vector2i& coord_tile, ItemCode item_code){

    sf::Vector2i chunk = world->ChunkFromCoord(coord_tile.x, coord_tile.y);

    if(world->ChunkInBounds(chunk.x, chunk.y)){
        
        UtilityStation* station;
        
        switch(ItemDictionary::ITEM_DATA[item_code].code_in_type){


            case utility_Furnace:
                station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<Chest>();
                break;

             case utility_Chest:
                station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<Chest>();
                break;               
        }




        if(station == nullptr){
            return;
        }
        station->LinkChunk(world->GetChunks()->at(chunk.x)[chunk.y]);
        station->OnStart();
        
        station->SetItemType((ItemCode)item_code);
        station->GetTransform()->position = sf::Vector2f(rounded_world.x, rounded_world.y);
        inventory->DecrementSelectedSlot();
    }
}
