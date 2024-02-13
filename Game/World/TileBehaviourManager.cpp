#include "TileBehaviourManager.h"


int TileBehaviourManager::tickrate = 50;
World* TileBehaviourManager::world;

void TileBehaviourManager::LinkWorld(World* _world){
    world = _world;
}

void TileBehaviourManager::PerformTileUpdatePass(){

    int width = world->world_profile.width * world->world_profile.tilemap_profile.width;
    int height = world->world_profile.height * world->world_profile.tilemap_profile.height;

    return;

    for(int i = 0; i < round(tickrate * Time::Dt()); i++){
        UpdateTile(rand() % width, rand() % height);
    }

}

void TileBehaviourManager::UpdateTile(int x, int y){

    EntireTile tile = world->GetEntireTile(x, y);
    
    switch(tile.foreground){

        case foreground_Leaves:
        case foreground_Moss:

            // grow grass
            if(world->GetTile(x, y - 1, SetLocation::MAIN) == -1 && world->GetTile(x, y, SetLocation::MAIN) != -1){
                world->SetTile(foreground_Grass, x, y - 1, SetLocation::FOREGROUND);
            }      
            // grow vine
            else if(world->GetTile(x, y + 1, SetLocation::MAIN) == -1 && world->GetTile(x, y + 1, SetLocation::FOREGROUND) == -1){

                // choose vine type
                if(rand() % 100 > 50){
                    world->SetTile(foreground_Vine, x, y + 1, SetLocation::FOREGROUND);
                }
                else{
                    world->SetTile(foreground_Roots, x, y + 1, SetLocation::FOREGROUND);
                }
            }    
            break;

        // vine types
        case foreground_Vine:

            if(world->GetTile(x, y + 1, SetLocation::MAIN) == -1 && world->GetTile(x, y + 1, SetLocation::FOREGROUND) == -1){

                // 35% chance vine ends early
                if(rand() % 100 < 35){
                                    // vine end
                    world->SetTile(foreground_Vine, x, y + 1, SetLocation::FOREGROUND);
                }
                if(rand() % 100 < 50){
                    world->SetTile(foreground_VineBerry, x, y + 1, SetLocation::FOREGROUND);
                }
                else{               // vine itself
                    world->SetTile(foreground_VineEnd, x, y + 1, SetLocation::FOREGROUND);
                }
            }

            break;

        case foreground_Roots:

            if(world->GetTile(x, y + 1, SetLocation::MAIN) == -1 && world->GetTile(x, y + 1, SetLocation::FOREGROUND) == -1){

                // 35% chance vine ends early
                if(rand() % 100 < 35){
                                    // vine end
                    world->SetTile(foreground_Roots, x, y + 1, SetLocation::FOREGROUND);
                }
                else{               // vine itself
                    world->SetTile(foreground_RootsEnd, x, y + 1, SetLocation::FOREGROUND);
                }
            }

            break;
    };
    switch(tile.main){
        case main_Dirt: 
            
            if(rand() % 100 > 40){
                if(world->GetTile(x, y - 1, SetLocation::FOREGROUND) == -1 && world->GetTile(x, y - 1, SetLocation::MAIN) == -1){
                    world->SetTile(foreground_Mushroom, x, y - 1, SetLocation::FOREGROUND);
                }
            }

            break;
    }
}

void TileBehaviourManager::PropogateTile(int x, int y, signed_byte block, signed_byte previous_block, SetLocation set_location){

    sf::Vector2i chunk_coord = world->ChunkFromCoord(x, y);
    sf::Vector2i chunk_pos = world->OffsetFromCoord(x, y, chunk_coord.x, chunk_coord.y);
    Chunk* chunk = world->GetChunks()->at(chunk_coord.x).at(chunk_coord.y);

    EntireTile entire_tile = chunk->GetEntireTile(chunk_pos.x, chunk_pos.y);
    EntireTile entire_tile_below = world->GetEntireTile(x, y + 1);
    EntireTile entire_tile_above = world->GetEntireTile(x, y - 1);

    ForegroundBehaviour behaviour = ItemDictionary::FOREGROUND_BLOCK_DATA[entire_tile.foreground].behaviour;
    ForegroundBehaviour behaviour_below = ItemDictionary::FOREGROUND_BLOCK_DATA[entire_tile_below.foreground].behaviour;
    ForegroundBehaviour behaviour_above = ItemDictionary::FOREGROUND_BLOCK_DATA[entire_tile_above.foreground].behaviour;

    // updating torches if necassary
    UpdateTorch(x, y);
    UpdateTorch(x - 1, y);
    UpdateTorch(x + 1, y);
    UpdateTorch(x, y - 1);
    UpdateTorch(x, y + 1);

    // establishing light sources if needed
    if(set_location == SetLocation::FOREGROUND){
        
        if(previous_block == foreground_Log){
            CutdownTree(x, y);
        }

        switch(previous_block){

            case foreground_Torch:
                chunk->RemoveTorchPosition(chunk_pos.x, chunk_pos.y);
                break;

        }

        switch(block){
            case foreground_Torch:{
                
                chunk->AddTorchPosition(chunk_pos.x, chunk_pos.y);
                break;
            }
        }
    }

    // changing tiles depending on foreground tile behaviour
    if(set_location == SetLocation::MAIN){

        UpdatePlatform(x, y);
        UpdatePlatform(x - 1, y);
        UpdatePlatform(x + 1, y);
        UpdatePlatform(x, y - 1);
        UpdatePlatform(x, y + 1);

        if(behaviour == ForegroundBehaviour::VINE){
            RemoveVine(x, y);
        }
        if(behaviour_below == ForegroundBehaviour::VINE){
            RemoveVine(x, y + 1);
        }


        if(block == -1){

            // remove any foliage that may be present
            chunk->RemoveFoliage(chunk_pos.x, chunk_pos.y);

            // remove any on block foreground tiles
            if(behaviour == ForegroundBehaviour::ON_BLOCK){
                world->SetTile(-1, x, y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, false, true);
            }

            // remove any above block foreground tiles if the above main block is also empty
            if(entire_tile_above.main == -1){

                if(behaviour_above == ForegroundBehaviour::ABOVE_BLOCK){
                    world->SetTile(-1, x, y - 1, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, false, true);
                }
            }
        }
        else{ // we placed a block

            if(behaviour == ForegroundBehaviour::ABOVE_BLOCK){
                world->SetTile(-1, x, y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, false, true);
            }
        }

    }


}

void TileBehaviourManager::RemoveVine(int x, int y){

    signed_byte tile_found = world->GetTile(x, y, SetLocation::FOREGROUND);
    // traverses down the vine until 

    while(ItemDictionary::FOREGROUND_BLOCK_DATA[tile_found].behaviour == ForegroundBehaviour::VINE){
        
        if(tile_found == -1){
            break;
        }

        world->SetTile(-1, x, y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, false, true);

        y++; // move down the vine

        tile_found = world->GetTile(x, y, SetLocation::FOREGROUND);
    }

}

void TileBehaviourManager::CutdownTree(int x, int y){
    signed_byte tile_found = world->GetTile(x, y - 1, SetLocation::FOREGROUND);

    while(tile_found == foreground_Log){
        
        world->SetTile(-1, x, y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, true, true);

        y--;
        tile_found = world->GetTile(x, y, SetLocation::FOREGROUND);
    }
}

void TileBehaviourManager::UpdatePlatform(int x, int y){

    signed_byte tile_found = world->GetTile(x, y, SetLocation::MAIN);

    if(tile_found <= -1){
        return;
    }

    // is actually a platform
    if(ItemDictionary::MAIN_BLOCK_DATA[tile_found].behaviour == ForegroundBehaviour::PLATFORM){
        
        // which platform sprite / tile should be shown?

        bool left = false;
        bool right = false;

        signed_byte left_tile = world->GetTile(x - 1, y);
        if(left_tile > -1 && ItemDictionary::MAIN_BLOCK_DATA[left_tile].behaviour != ForegroundBehaviour::PLATFORM){
            left = true;
        }
        signed_byte right_tile = world->GetTile(x + 1, y);
        if(right_tile > -1 && ItemDictionary::MAIN_BLOCK_DATA[right_tile].behaviour != ForegroundBehaviour::PLATFORM){
            right = true;
        }       

        if(left && right){
            world->SetTile(main_Platform, x, y, SetLocation::MAIN);
        }
        else if(left){
            world->SetTile(main_PlatformLeftSupported, x, y, SetLocation::MAIN);
        }
        else if(right){
            world->SetTile(main_PlatformRightSupported, x, y, SetLocation::MAIN);
        }
        else{
            world->SetTile(main_Platform, x, y, SetLocation::MAIN);
        }
    }
}

void TileBehaviourManager::UpdateTorch(int x, int y){

    signed_byte tile_found = world->GetTile(x, y, SetLocation::FOREGROUND);

    if(ItemDictionary::FOREGROUND_BLOCK_DATA[tile_found].behaviour == ForegroundBehaviour::TORCH){

        if(world->GetTile(x, y) == -1){

            // left
            if(world->GetTile(x - 1, y) > -1){
                world->SetTile(foreground_TorchLeft, x, y, SetLocation::FOREGROUND);
                return;
            }
            // right
            else if(world->GetTile(x + 1, y) > -1){
                world->SetTile(foreground_TorchRight, x, y, SetLocation::FOREGROUND);
                return;
            }
            // has background or block below
            else if(world->GetTile(x, y, SetLocation::BACKGROUND) > -1){
                world->SetTile(foreground_Torch, x, y, SetLocation::FOREGROUND);
                return;
            }
            else if(world->GetTile(x, y + 1) > -1){
                world->SetTile(foreground_Torch, x, y, SetLocation::FOREGROUND);
                return;
            }
        }

        // torch position has become invalid, remove
        world->SetTile(-1, x, y, SetLocation::FOREGROUND, SetMode::OVERRIDE, true, false, true);
    }
}