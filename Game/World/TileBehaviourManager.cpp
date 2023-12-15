#include "TileBehaviourManager.h"


int TileBehaviourManager::tickrate = 50;
World* TileBehaviourManager::world;

void TileBehaviourManager::LinkWorld(World* _world){
    world = _world;
}

void TileBehaviourManager::PerformTileUpdatePass(){

    int width = world->world_profile.width * world->world_profile.tilemap_profile.width;
    int height = world->world_profile.height * world->world_profile.tilemap_profile.height;

    for(int i = 0; i < round(tickrate * Time::Dt()); i++){
        UpdateTile(rand() % width, rand() % height);
    }

}

void TileBehaviourManager::UpdateTile(int x, int y){

    signed_byte foreground_tile = world->GetTile(x, y, SetLocation::FOREGROUND);
    
    switch(foreground_tile){

        case foreground_Leaves:
        case foreground_Moss:

            // grow grass
            if(world->GetTile(x, y - 1, SetLocation::MAIN) == -1 && world->GetTile(x, y, SetLocation::MAIN) != -1){
                world->SetTile(foreground_Grass, x, y - 1, SetLocation::FOREGROUND);
            }      
            // grow vine
            else if(world->GetTile(x, y + 1, SetLocation::MAIN) == -1){
                world->SetTile(foreground_Vine, x, y + 1, SetLocation::FOREGROUND);
            }    
            break;

        case foreground_Vine:

            if(world->GetTile(x, y + 1, SetLocation::MAIN) == -1 && world->GetTile(x, y + 1, SetLocation::FOREGROUND) == -1){

                // 20% chance vine ends early
                if(rand() % 100 < 20){
                    world->SetTile(foreground_VineEnd, x, y + 1, SetLocation::FOREGROUND);
                }
                else{
                    world->SetTile(foreground_Vine, x, y + 1, SetLocation::FOREGROUND);
                }
            }

            break;
    };

}