#include "WaterManager.h"
#include <algorithm>
#include "World.h"

short WaterManager::awake_decay = 4;
std::vector<int> WaterManager::awake_chunks;
float WaterManager::water_update_delay = 50;
World* WaterManager::world;

void WaterManager::LinkWorld(World* _world){
    world = _world;
    awake_chunks.clear();
}

void WaterManager::AddNewAwakeChunk(const sf::Vector2i& chunk_coord){


    int chunk_index = chunk_coord.x  + chunk_coord.y  * world->GetWorldProfile()->width;

    if(std::find(awake_chunks.begin(), awake_chunks.end(), chunk_index) == awake_chunks.end()){

        awake_chunks.push_back(chunk_index);
        // sort descending
        std::sort(awake_chunks.begin(), awake_chunks.end(), std::greater<>());
    }
}

void WaterManager::AwakenSurroundingChunks(Chunk* chunk){

    sf::Vector2i offsets[9] = {sf::Vector2i(-1,1),sf::Vector2i(0,1), sf::Vector2i(1,1),
                               sf::Vector2i(-1,0), sf::Vector2i(0,0), sf::Vector2i(1,0),
                               sf::Vector2i(-1,-1), sf::Vector2i(0,-1), sf::Vector2i(1,-1)};


    for(int i = 0; i < 9; i++){

        if(!world->ChunkInBounds(chunk->GetChunkCoordinate().x + offsets[i].x, chunk->GetChunkCoordinate().y + offsets[i].y)){
            continue;
        }

        world->GetChunks()->at(chunk->GetChunkCoordinate().x + offsets[i].x).at(chunk->GetChunkCoordinate().y + offsets[i].y)->SetAwakeForWaterSim(awake_decay);
    }
}


void WaterManager::RemoveAwakeChunk(const sf::Vector2i& chunk_coord){

    int chunk_index = chunk_coord.x + chunk_coord.y * world->GetWorldProfile()->width;

    for(int i = 0; i < awake_chunks.size(); i++){
        if(awake_chunks[i] == chunk_index){
            awake_chunks.erase(awake_chunks.begin() + i);
        }
    }

}

void WaterManager::Update(){
    
/*
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Q)){
            sf::Vector2f world_pos = Scene::GetActiveCamera()->ScreenToWorldPosition(Mouse::DisplayPosition());
        sf::Vector2i coord = world->WorldToCoord(world_pos.x, world_pos.y);

        sf::Vector2i chunk_coord = world->ChunkFromCoord(coord.x, coord.y);
        sf::Vector2i chunk_offset = world->OffsetFromCoord(coord.x, coord.y, chunk_coord.x, chunk_coord.y);

        world->GetChunks()->at(chunk_coord.x).at(chunk_coord.y)->water_tilemap->SetTile(water_full, chunk_offset.x, chunk_offset.y);
        world->GetChunks()->at(chunk_coord.x).at(chunk_coord.y)->SetAwakeForWaterSim(awake_decay);
    }
*/


    water_update_delay_tracked += Time::Dt();

    if(water_update_delay_tracked > water_update_delay){

        water_update_delay_tracked = 0;

        for(int i = 0; i < awake_chunks.size(); i++){


            int chunk_x = awake_chunks[i] % world->GetWorldProfile()->width;
            int chunk_y = floor((awake_chunks[i] - chunk_x) / (float)world->GetWorldProfile()->width);


            Chunk* chunk = world->GetChunks()->at(chunk_x).at(chunk_y);


            chunk->water_updated.clear();
            chunk->water_updated.resize(world->GetWorldProfile()->tilemap_profile.width, {});
            for(int x = 0; x < world->GetWorldProfile()->tilemap_profile.width; x++){
                chunk->water_updated[x].resize(world->GetWorldProfile()->tilemap_profile.height, false);
            }

            bool stay_awake = false;

            sf::Vector2i chunk_tile_coordinate = sf::Vector2i(chunk->GetChunkCoordinate().x * world->GetWorldProfile()->tilemap_profile.width, chunk->GetChunkCoordinate().y * world->GetWorldProfile()->tilemap_profile.height);


            for(int x = 0; x < world->GetWorldProfile()->tilemap_profile.width; x++){
                for(int y = world->GetWorldProfile()->tilemap_profile.height - 1; y >= 0; y--){
                
                    // current cell has not been updated
                    if(!chunk->water_updated[x][y]){

                        if(chunk->water_tilemap->GetTile(x, y) != -1){

                            //Tilemap* main_tilemap = chunk->GetTilemap(SetLocation::MAIN);
                            //Tilemap* water_tilemap = chunk->GetTilemap(SetLocation::WATER);

                                /*                 
                            // we are within the center of a chunk
                            if(x > 0 && x < world->GetWorldProfile()->tilemap_profile.width - 1 &&
                               y > 0 && y < world->GetWorldProfile()->tilemap_profile.height - 1){

                                if(main_tilemap->GetTile(x, y + 1) == -1 && water_tilemap->GetTile(x, y + 1) == -1){
                                    SwapTile(chunk, x, y, x, y + 1);
                                    stay_awake = true;
                                }
                                else if(main_tilemap->GetTile(x - 1, y + 1) == -1 && water_tilemap->GetTile(x - 1, y + 1) == -1){
                                    SwapTile(chunk, x, y, x - 1, y + 1);
                                    stay_awake = true;
                                }
                                else if(main_tilemap->GetTile(x + 1, y + 1) == -1 && water_tilemap->GetTile(x + 1, y + 1) == -1){
                                    SwapTile(chunk, x, y, x + 1, y + 1); 
                                    stay_awake = true;                      
                                }
                                else if(main_tilemap->GetTile(x - 1, y) == -1 && water_tilemap->GetTile(x - 1, y) == -1){
                                    SwapTile(chunk, x, y, x - 1, y);
                                    stay_awake = true;
                                }
                                else if(main_tilemap->GetTile(x + 1, y) == -1 && water_tilemap->GetTile(x + 1, y) == -1){
                                    SwapTile(chunk, x, y, x + 1, y);        
                                    stay_awake = true;              
                                }
                            }       
                            */   
                            if(true){ // we are crossing a chunk :(
    

                                int _x = chunk_tile_coordinate.x + x;
                                int _y = chunk_tile_coordinate.y + y;

                                
                                if(world->GetTile(_x, _y + 1) == -1 && world->GetTile(_x, _y + 1, SetLocation::WATER) == -1){
                                    world->SetTile(-1, _x, _y, SetLocation::WATER);
                                    world->SetTile(water_full, _x, _y + 1, SetLocation::WATER); 
                                    stay_awake = true;
                                }
                                else if(world->GetTile(_x - 1, _y + 1) == -1 && world->GetTile(_x - 1, _y + 1, SetLocation::WATER) == -1){
                                    world->SetTile(-1, _x, _y, SetLocation::WATER);
                                    world->SetTile(water_full, _x - 1, _y + 1, SetLocation::WATER);
                                    stay_awake = true;
                                }
                                else if(world->GetTile(_x + 1, _y + 1) == -1 && world->GetTile(_x + 1, _y + 1, SetLocation::WATER) == -1){
                                    world->SetTile(-1, _x, _y, SetLocation::WATER);
                                    world->SetTile(water_full, _x + 1, _y + 1, SetLocation::WATER);    
                                    stay_awake = true;             
                                }
                                else if(world->GetTile(_x - 1, _y) == -1 && world->GetTile(_x - 1, _y, SetLocation::WATER) == -1){
                                    world->SetTile(-1, _x, _y, SetLocation::WATER);
                                    world->SetTile(water_full, _x - 1, _y, SetLocation::WATER);
                                    stay_awake = true;
                                }
                                else if(world->GetTile(_x + 1, _y) == -1 && world->GetTile(_x + 1, _y, SetLocation::WATER) == -1){
                                    world->SetTile(-1, _x, _y, SetLocation::WATER);
                                    world->SetTile(water_full, _x + 1, _y, SetLocation::WATER);                 
                                    stay_awake = true;
                                }

                            }                  
                        }
                    }
                }
            }

            if(!stay_awake){
                chunk->SetAwakeForWaterSim(chunk->GetAwakeForWaterSim() - 1);

                if(chunk->GetAwakeForWaterSim() <= 0){
                    RemoveAwakeChunk(chunk->GetChunkCoordinate());
                }
            }
            else{
                AwakenSurroundingChunks(chunk);
            }

        }

    }
}

void WaterManager::SwapTile(Chunk* chunk, int from_x, int from_y, int new_x, int new_y){
    chunk->water_tilemap->SetTile(-1, from_x, from_y);
    chunk->water_tilemap->SetTile(water_full, new_x, new_y);    
    chunk->water_updated[new_x][new_y] = true;
}

