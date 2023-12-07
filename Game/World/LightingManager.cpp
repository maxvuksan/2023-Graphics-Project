#include "LightingManager.h"
#include "World.h"
#include "../Player/LightSource.h"

World* LightingManager::world;
std::vector<LightSource*> LightingManager::light_sources;

void LightingManager::LinkWorld(World* _world){
    world = _world;
}

void LightingManager::Update(){

    sf::Vector2f pos;
    for(int i = 0; i < light_sources.size(); i++){
        pos = light_sources[i]->GetThisObject()->GetTransform()->position;
        PropogateLighting(world->WorldToCoord(pos.x, pos.y));
    }
}

void LightingManager::PropogateLighting(sf::Vector2i coordinate, float light_value){
    
    if(light_value < 0.1f){
        return;
    }
    if(!world->CoordInBounds(coordinate.x, coordinate.y)){
        return;
    }

    sf::Vector2i chunk_coord = world->ChunkFromCoord(coordinate.x, coordinate.y);
    std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();
    
    Chunk* chunk = chunks->at(chunk_coord.x).at(chunk_coord.y);

    if(!chunk->IsActive()){
        return;
    }


    signed_byte current_tile = world->GetTile(coordinate.x, coordinate.y, SetLocation::MAIN);

    if(current_tile != -1){
        light_value *= 0.5f;
    }


    // coordinate within said chunk
    sf::Vector2i light_map_coord = world->OffsetFromCoord(coordinate.x, coordinate.y, chunk_coord.x, chunk_coord.y);

    sf::Image& light_map = chunk->GetLightmap();
    
    sf::Color new_col(255 * light_value, 255 * light_value,255 * light_value);

    
    if(light_map.getPixel(light_map_coord.x, light_map_coord.y).r < new_col.r){

        light_map.setPixel(light_map_coord.x, light_map_coord.y, new_col);
    
        chunk->MarkLightmapDirty();

        if(current_tile != -1){
            if(world->GetTile(coordinate.x, coordinate.y + 1, SetLocation::MAIN) == -1){
                PropogateLighting(coordinate + sf::Vector2i(0, 1), light_value * 0.93f);
            }
            if(world->GetTile(coordinate.x, coordinate.y - 1, SetLocation::MAIN) == -1){
                PropogateLighting(coordinate + sf::Vector2i(0, -1), light_value * 0.93f);
            }
            if(world->GetTile(coordinate.x + 1, coordinate.y, SetLocation::MAIN) == -1){
                PropogateLighting(coordinate + sf::Vector2i(1,0), light_value * 0.93f);
            }
            if(world->GetTile(coordinate.x - 1, coordinate.y + 1, SetLocation::MAIN) == -1){
                PropogateLighting(coordinate + sf::Vector2i(-1, 0), light_value * 0.93f);
            }
        }
        else{
            PropogateLighting(coordinate + sf::Vector2i(0, 1), light_value * 0.93f);
            PropogateLighting(coordinate + sf::Vector2i(0, -1), light_value * 0.93f);
            PropogateLighting(coordinate + sf::Vector2i(1,0), light_value * 0.93f);
            PropogateLighting(coordinate + sf::Vector2i(-1, 0), light_value * 0.93f);
        }
    }
}

void LightingManager::AddLightSource(LightSource* light_source){
    light_sources.push_back(light_source);
}

void LightingManager::DeleteLightSource(LightSource* light_source){
    for(int i = 0; i < light_sources.size(); i++){
        if(light_sources[i] == light_source){
            light_sources.erase(light_sources.begin() + i);
        }
    }
}

