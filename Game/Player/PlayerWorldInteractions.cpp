#include "PlayerWorldInteractions.h"
#include "../World/World.h"
#include "Inventory.h"
#include "../Utility/UtilityStation.h"
#include "../Utility/Container.h"
#include "../Utility/CraftingStation.h"
#include "Projectile.h"
#include "HealthBar.h"

void PlayerWorldInteractions::Start(){
    
    SetRenderLayer(9);

    cursor_graphic = object->GetScene()->AddUI<CursorGraphic>();
    selected_block = 0;
    auto_target_blocks = false;

    ItemDictionary::SetItemSprite(tool_in_hand_sprite, ItemCode::item_Copper_Sword);
    last_swing_value = 0;
}

void PlayerWorldInteractions::LinkWorld(World* world){
    this->world = world;
}
void PlayerWorldInteractions::LinkInventory(Inventory* inventory){
    this->inventory = inventory;


}
void PlayerWorldInteractions::LinkHealthBar(HealthBar* health_bar){
    this->health_bar = health_bar;
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
    ManageToolInHand(surface);
}

void PlayerWorldInteractions::ManageToolInHand(sf::RenderTarget& surface){


    if(swinging_tool){
        
        swing_completion -= Time::Dt() * 0.002f;

        if(!Calc::InRange(swing_completion, 0, 1)){
            swinging_tool = false;
        }

    }

    float angle_to_mouse = Calc::AngleBetween(Camera::WorldToScreenPosition(object->GetTransform()->position), sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y));

    sf::Vector2f tool_offset(cos(Calc::Radians(angle_to_mouse + new_swing_rotation)) * 10, sin(Calc::Radians(angle_to_mouse + new_swing_rotation)) * 10);

    tool_in_hand_sprite.setPosition(Camera::WorldToScreenPosition(object->GetTransform()->position) + tool_offset);
    tool_in_hand_sprite.setRotation(angle_to_mouse + old_swing_rotation);

    surface.draw(tool_in_hand_sprite);
}

void PlayerWorldInteractions::SwingToolInHand(){

    swinging_tool = true;
    swing_completion = 1;

    if(last_swing_value == 0){
        last_swing_value = 1;
        
        new_swing_rotation = 90;
        old_swing_rotation = 200;
        overswing_multiplier = -20;
    }
    else{
        last_swing_value = 0;

        old_swing_rotation = -20;
        new_swing_rotation = -90;
        overswing_multiplier = 20;
    }
    
    Object* projectile = GetThisObject()->GetScene()->AddObject<Projectile>();
    projectile->GetTransform()->rotation = Calc::AngleBetween(Camera::WorldToScreenPosition(object->GetTransform()->position), sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y));
    projectile->GetTransform()->position = object->GetTransform()->position;
}

void PlayerWorldInteractions::CatchEvent(sf::Event event){
    
    if (event.type == sf::Event::KeyPressed)
    {
        if(event.key.scancode == sf::Keyboard::Scan::LAlt){
            
            auto_target_blocks = !auto_target_blocks;
            cursor_graphic->SetAutoTargetBlocks(auto_target_blocks);
        }

    }
    if(event.type == sf::Event::MouseButtonPressed){
        
        // right clicking with an item in hand
        if(event.mouseButton.button == sf::Mouse::Button::Right){
            int item_in_hand = inventory->GetItemInSelectedSlot();

            if(item_in_hand == -1){
                return;
            }

            const ItemData* data = &ItemDictionary::ITEM_DATA[item_in_hand];
            
            // eat food
            if(data->type == type_Food){
                std::cout << "eat\n";
                health_bar->SetHunger(health_bar->GetHunger() + ItemDictionary::FOOD_DATA[data->code_in_type].hunger_to_add);
                health_bar->SetHealth(health_bar->GetHealth() + ItemDictionary::FOOD_DATA[data->code_in_type].health_to_add);
                Sound::Play("player_damage");
            }
        }
        SwingToolInHand();
    }
}

void PlayerWorldInteractions::CalculateMouse(sf::RenderTarget& surface){

    cursor_graphic->SetActive(true);

    sf::Vector2i _mouse_pos = Mouse::DisplayPosition();
    sf::Vector2f player_center = object->GetScene()->GetActiveCamera()->WorldToScreenPosition(object->GetTransform()->position);

    _mouse_pos.x -= round(player_center.x);
    _mouse_pos.y -= round(player_center.y);

    mouse_world_pos = sf::Vector2f(object->GetTransform()->position.x + _mouse_pos.x, object->GetTransform()->position.y + _mouse_pos.y );


    sf::Vector2f world_tile = mouse_world_pos;
    sf::Vector2i coord_tile = world->WorldToCoord(mouse_world_pos.x, mouse_world_pos.y);

    cursor_graphic->GetTransform()->position = sf::Vector2f(mouse_world_pos.x, mouse_world_pos.y);


    int item_in_hand = inventory->GetItemInSelectedSlot();
    
    if(item_in_hand == -1){
        return;
    }

    const ItemData* data = &ItemDictionary::ITEM_DATA[item_in_hand];
    
    SetLocation set_location = SetLocation::MAIN;
    if(data->type == ItemType::type_Hammer){
        set_location = SetLocation::BACKGROUND;
    }
    else if(data->type == ItemType::type_Axe){
        set_location = SetLocation::FOREGROUND;
    }

    // should we consider auto target mode?
    if(auto_target_blocks && (data->type == ItemType::type_Hammer || data->type == ItemType::type_Picaxe)){

        sf::Vector2i auto_tile = world->GetNearestTileInDirectionOfMouse(object->GetTransform()->position, set_location);
        
        if(auto_tile != sf::Vector2i(-1,-1)){
            coord_tile = auto_tile;
            // convert auto coord to world space
            world_tile = world->CoordToWorld(coord_tile.x, coord_tile.y);
        }
    }

    sf::Vector2i rounded_world = world->RoundWorld(world_tile.x, world_tile.y);
    cursor_graphic->SetBreakingPosition(sf::Vector2f(rounded_world.x, rounded_world.y));


    focused_block = world->GetTile(coord_tile.x, coord_tile.y, set_location);
    
    Mine(coord_tile, rounded_world, (ItemCode)item_in_hand);
        
    
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

        if(NewUtilityOverlaps(coord_tile, rounded_world, ItemDictionary::UTILITY_BLOCK_DATA[data->code_in_type].footprint) != nullptr){
            utility_location_valid = false;
        }

        if(utility_location_valid){
            utility_hologram.setColor(sf::Color(255,255,255,180));
        }
        else{
            utility_hologram.setColor(sf::Color(255,90,90,180));
        }
        utility_hologram.setPosition(Camera::WorldToScreenPosition(sf::Vector2f(rounded_world.x, rounded_world.y)));

        surface.draw(utility_hologram);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        if(focused_block != -1){
            return;
        }


        if(world->CoordIsConnectedToOtherTiles(coord_tile.x, coord_tile.y)){

            switch(data->type){

                case ItemType::type_Main: {
                    
                    // would this new block overlap any utility stations
                    if(NewUtilityOverlaps(coord_tile, rounded_world, sf::Vector2i(1,1)) == nullptr){

                        if(world->SetTile(data->code_in_type, coord_tile.x, coord_tile.y, SetLocation::MAIN, SetMode::OVERRIDE, true, true)){
                            inventory->DecrementSelectedSlot();
                        }
                    }
                    break;
                }
                case ItemType::type_Foreground: {

                    if(world->GetTile(coord_tile.x, coord_tile.y, SetLocation::FOREGROUND) == -1){
                        if(world->SetTile(data->code_in_type, coord_tile.x, coord_tile.y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, true)){
                            inventory->DecrementSelectedSlot();
                        }
                    }
                    break;
                }
                case ItemType::type_Background: {

                    if(world->GetTile(coord_tile.x, coord_tile.y, SetLocation::BACKGROUND) == -1){
                        if(world->SetTile(data->code_in_type, coord_tile.x, coord_tile.y, SetLocation::BACKGROUND, SetMode::OVERRIDE, true, true)){
                            inventory->DecrementSelectedSlot();
                        }
                    }
                    break;
                }
            }
        }


        if(data->type == ItemType::type_Utility){

            if(utility_location_valid){
                
                PlaceUtility(coord_tile, rounded_world, (ItemCode)item_in_hand);
            }
        }

    }

    previous_item_code = (ItemCode)item_in_hand;
}

UtilityStation* PlayerWorldInteractions::NewUtilityOverlaps(const sf::Vector2i& coord_tile, const sf::Vector2i& rounded_world, sf::Vector2i footprint, bool assign_last_overlap_chunk){
    
    // checks each corner of the utility object to determine all the chunks it overlaps with, 

    std::vector<sf::Vector2i> chunks_to_check;
        
    chunks_to_check.push_back(world->ChunkFromCoord(coord_tile.x, coord_tile.y));
    
    sf::Vector2i top_right_corner = world->ChunkFromCoord(coord_tile.x + footprint.x, coord_tile.y);
    if(std::find(chunks_to_check.begin(), chunks_to_check.end(), top_right_corner) != chunks_to_check.end()){
        
        chunks_to_check.push_back(top_right_corner);
    }
    sf::Vector2i bottom_right_corner = world->ChunkFromCoord(coord_tile.x + footprint.x, coord_tile.y + footprint.y);
    if(std::find(chunks_to_check.begin(), chunks_to_check.end(), bottom_right_corner) != chunks_to_check.end()){
        
        chunks_to_check.push_back(top_right_corner);
    }

    sf::Vector2i bottom_left_corner = world->ChunkFromCoord(coord_tile.x, coord_tile.y + footprint.y);
    if(std::find(chunks_to_check.begin(), chunks_to_check.end(), bottom_left_corner) != chunks_to_check.end()){
        chunks_to_check.push_back(top_right_corner);
    }

    // check overlaps of each utility object in each chunk







    for(int i = 0; i < chunks_to_check.size(); i++){

        if(!world->ChunkInBounds(chunks_to_check[i].x, chunks_to_check[i].y)){
            continue;
        }

        if(assign_last_overlap_chunk){
            chunk_last_utility_overlap_was_in = world->GetChunks()->at(chunks_to_check[i].x).at(chunks_to_check[i].y);
        }

        std::vector<UtilityStation*>* stations_in_chunk =  world->GetChunks()->at(chunks_to_check[i].x).at(chunks_to_check[i].y)->GetUtilityStations();
    
        for(int u = 0; u < stations_in_chunk->size(); u++){
            if(stations_in_chunk->at(u)->FootprintOver(rounded_world, footprint)){

                return stations_in_chunk->at(u);
            }
        }
    }
    return nullptr;
}

void PlayerWorldInteractions::Mine(const sf::Vector2i& coord_tile, const sf::Vector2i& rounded_world, ItemCode item_code){
    
    // we have switched tools
    if(previous_item_code != item_code){
        cursor_graphic->SetComplete(0);
        breaking_completeness = 0;
        sound_increment = 0;
    }


    float mining_speed = 1;
    const ItemData* data = &ItemDictionary::ITEM_DATA[item_code];

    UtilityStation* utility_to_mine = nullptr;

    // get the tool 
    if(data->type == ItemType::type_Picaxe){
        mining_speed = ItemDictionary::PICAXE_DATA[data->code_in_type].speed;
        
        utility_to_mine = NewUtilityOverlaps(coord_tile, rounded_world, sf::Vector2i(1,1), true);
    }
    else if(data->type == ItemType::type_Hammer){
        mining_speed = ItemDictionary::HAMMER_DATA[data->code_in_type].speed;
    }
    else if(data->type == ItemType::type_Axe){
        mining_speed = ItemDictionary::AXE_DATA[data->code_in_type].speed;
    }
    else{
        return;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        if(rounded_world != focused_block_position){
            focused_block_position = rounded_world;

            breaking_completeness = 0;
            sound_increment = 0;
        }

        // mining block
        if(focused_block != -1){

            // multiply the tool speed by the durability of the block
            float speed_of_specific_tile = (float)ItemDictionary::MAIN_BLOCK_DATA[focused_block].durability;
            if(data->type == ItemType::type_Picaxe){
                speed_of_specific_tile = (float)ItemDictionary::MAIN_BLOCK_DATA[focused_block].durability;
            }
            else if(data->type == ItemType::type_Hammer){
                speed_of_specific_tile = (float)ItemDictionary::BACKGROUND_BLOCK_DATA[focused_block].durability;
            }
            else if(data->type == ItemType::type_Axe){
                speed_of_specific_tile = (float)ItemDictionary::FOREGROUND_BLOCK_DATA[focused_block].durability;              
            }


            float increment = mining_speed * 0.01f * Time::Dt() / speed_of_specific_tile;
            breaking_completeness += increment;
            sound_increment += increment;

        }

        else if(utility_to_mine != nullptr){
            float increment = mining_speed * 0.01f * Time::Dt() / utility_to_mine->GetUtilityData()->block_data.durability;
            breaking_completeness += increment;
        }

        cursor_graphic->SetComplete(breaking_completeness);

        if(breaking_completeness >= 1){

            // we broke a utility station
            if(utility_to_mine != nullptr){

                if(utility_to_mine->CanBreak()){

                    Sound::Play("break", 10);
                    chunk_last_utility_overlap_was_in->BreakUtilityStation(utility_to_mine);

                    focused_block_position = sf::Vector2i(-1,-1);
                }
            }

            else{
                // assume we have a picaxe
                SetLocation set_location = SetLocation::MAIN;
                if(data->type == ItemType::type_Hammer){
                    set_location = SetLocation::BACKGROUND;
                }
                if(data->type == ItemType::type_Axe){
                    set_location = SetLocation::FOREGROUND;
                }


                // break utility station above block as well
                bool break_allowed = true;

                if(set_location == SetLocation::MAIN){
                    UtilityStation* station = NewUtilityOverlaps(coord_tile + sf::Vector2i(0, -1), rounded_world + sf::Vector2i(0,-ItemDictionary::tile_size), sf::Vector2i(1,1), false);
                    
                    if(station != nullptr){
                        
                        if(!station->CanBreak()){
                            break_allowed = false;
                        }
                        else{
                            chunk_last_utility_overlap_was_in->BreakUtilityStation(station);
                        }
                    }
                }


                if(break_allowed){
                    // block breaks...
                    Sound::Play("break", 10);

                    world->SetTile(-1, coord_tile.x, coord_tile.y, set_location, SetMode::OVERRIDE, true, true, true);
                    focused_block_position = sf::Vector2i(-1,-1); // clearing the focused block (an impossible position)
                }
            }
        }
    }
    else{
        cursor_graphic->SetComplete(0);
        breaking_completeness = 0;
        sound_increment = 0;
    }
}

void PlayerWorldInteractions::PlaceUtility(const sf::Vector2i& coord_tile, const sf::Vector2i& rounded_world, ItemCode item_code){


    sf::Vector2i chunk = world->ChunkFromCoord(coord_tile.x, coord_tile.y);

    if(world->ChunkInBounds(chunk.x, chunk.y)){
        
        UtilityStation* station = nullptr;
        CraftingStation* crafting_station = nullptr;
        RecipeGroups recipe_group;

        switch(ItemDictionary::ITEM_DATA[item_code].code_in_type){


            case utility_WorkBench: {
                crafting_station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<CraftingStation>();
                recipe_group = rgroup_Workbench;
                station = crafting_station;
                break;
            }
            case utility_Furnace: {
                crafting_station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<CraftingStation>();
                recipe_group = rgroup_Furnace;
                station = crafting_station;
                break;
            }
            case utility_Anvil: {
                crafting_station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<CraftingStation>();
                recipe_group = rgroup_Anvil;
                station = crafting_station;
                break;
            }
            case utility_Cookpot: {
                crafting_station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<CraftingStation>();
                recipe_group = rgroup_Cookpot;
                station = crafting_station;
                break;
            }
            case utility_StoneCutter: {
                crafting_station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<CraftingStation>();
                recipe_group = rgroup_StoneCutter;
                station = crafting_station;
                break;
            }
            // containers 

            case utility_Chest: {
                station = world->GetChunks()->at(chunk.x)[chunk.y]->AddObjectToChunk<Container>();
                break;            
            }   
        }


        if(station == nullptr){
            return;
        }

        world->GetChunks()->at(chunk.x)[chunk.y]->StoreUtilityStationReference(station);
        station->LinkChunk(world->GetChunks()->at(chunk.x)[chunk.y]);
        station->OnStart();
        
        station->SetItemType((ItemCode)item_code);

        if(crafting_station != nullptr){
            crafting_station->SetRecipeGroup(recipe_group);
        }

        station->GetTransform()->position = sf::Vector2f(rounded_world.x, rounded_world.y);
        inventory->DecrementSelectedSlot();
    }
}
