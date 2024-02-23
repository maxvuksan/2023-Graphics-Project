#include "World.h"
#include <cmath>
#include "../Networking/GameClient.h"
#include "../Items/Pickup.h"
#include "../WorldScene.h"
#include "TileBehaviourManager.h"
#include "RotatedRectManager.h"
#include "WaterManager.h"

#include "Lighting/LightingManager.h"
#include "Lighting/BackgroundShadowManager.h"


void World::Create(bool minimal, int width, int height) {


    if(width != -1){
        world_profile.width = width;
    }
    if(height != -1){
        world_profile.height = height;
    }


    world_needs_pathfinding_recalculating = true;
    tilemap_profile = &world_profile.tilemap_profile;

    world_profile.width_in_tiles = world_profile.width * tilemap_profile->width;
    world_profile.height_in_tiles = world_profile.height * tilemap_profile->height;


    // creating each tilemap...

    half_tilemap_width = floor(tilemap_profile->width * 0.5f);
    half_tilemap_height = floor(tilemap_profile->height * 0.5f);
    one_divide_tilemap_width = 1 / (float)tilemap_profile->width;
    one_divide_tilemap_height = 1 / (float)tilemap_profile->height;

    // creating chunk vector...
    chunks.resize(world_profile.width);
    for(int x = 0; x < world_profile.width; x++){
        chunks[x].resize(world_profile.height, nullptr);
    }


    if(!minimal){
        GetScene()->AddObject<LightingManager>(50);
        GetScene()->AddObject<RotatedRectManager>();
        GetScene()->AddObject<WaterManager>();
        GetScene()->AddObject<BackgroundShadowManager>(1);
        
        TilemapCollisionTypeDivider::platform_collider_begins_at = main_Platform;
        LightingManager::LinkWorld(this);
        TileBehaviourManager::LinkWorld(this);
        RotatedRectManager::LinkWorld(this);
        WaterManager::LinkWorld(this);
    }

    LightingManager::ClearLightSources();

    minimap = GetScene()->AddUI<Minimap>();
    minimap->GetMainPixelGrid()->Create(world_profile.width_in_tiles, world_profile.height_in_tiles, sf::Color::Transparent);
    minimap->GetBackgroundPixelGrid()->Create(world_profile.width_in_tiles, world_profile.height_in_tiles, sf::Color::Transparent);
    minimap->GetForegroundPixelGrid()->Create(world_profile.width_in_tiles, world_profile.height_in_tiles, sf::Color::Transparent);
    minimap->GetExploredPixelGrid()->Create(world_profile.width_in_tiles, world_profile.height_in_tiles, sf::Color::Black);
    minimap->LinkWorld(this);

    for(int x = 0; x < world_profile.width; x++){
        for(int y = 0; y < world_profile.height; y++){

            // create chunk and place it in the correct position
            // chunks are not held in the scenes generic object array for performance reasons.
            // instead we keep it in a seperate 2d array and move ONLY nearby chunks into the scenes simulation
            // this allows disabled chunks to have little to no CPU involement, essentially being idle in RAM
            chunks[x][y] = Memory::New<Chunk>(__FUNCTION__);
            chunks[x][y]->LinkScene(GetScene());
            chunks[x][y]->Init(this);
            chunks[x][y]->SetActive(false); 
            chunks[x][y]->SetChunkCoordinate(x, y);
        }
    }


    world_has_been_created = true;

}

void World::Generate(){
    WorldGenerator::Bind(this);
    WorldGenerator::Generate();
}


void World::LinkClient(GameClient* client){
    this->client = client;
}



void World::CalculateMinimap(){

    for(int x = 0; x < chunks.size(); x++){
        for(int y = 0; y < chunks.at(x).size(); y++){

            for(int t_x = 0; t_x < tilemap_profile->width; t_x++){
                for(int t_y = 0; t_y < tilemap_profile->height; t_y++){
                    
                    int final_x = x * tilemap_profile->width + t_x;
                    int final_y = y * tilemap_profile->height + t_y;

                    int tile_main = chunks.at(x).at(y)->GetTile(t_x, t_y, SetLocation::MAIN);
                    int tile_foreground = chunks.at(x).at(y)->GetTile(t_x, t_y, SetLocation::FOREGROUND);
                    int tile_background = chunks.at(x).at(y)->GetTile(t_x, t_y, SetLocation::BACKGROUND);

                    DrawTileToMinimap(tile_main, final_x, final_y, SetLocation::MAIN);
                    DrawTileToMinimap(tile_foreground, final_x, final_y, SetLocation::FOREGROUND);
                    DrawTileToMinimap(tile_background, final_x, final_y, SetLocation::BACKGROUND);

                }
            }

        }
    }

}

bool World::SetTileWorld(signed_byte tile_index, float world_x, float world_y, SetLocation set_location, bool send_packet){

    sf::Vector2i coord = WorldToCoord(world_x, world_y);
    return SetTile(tile_index, coord.x, coord.y, set_location, SetMode::OVERRIDE, send_packet, true, true);
}

bool World::SetTile(signed_byte tile_index, int x, int y, SetLocation set_location, SetMode set_mode, bool send_packet, bool propogate_to_other_tiles, bool create_pickup){
    
    sf::Vector2i chunk = ChunkFromCoord(x, y);
    if(!ChunkInBounds(chunk.x, chunk.y)){
        return false;
    }

    if(set_mode != SetMode::OVERRIDE){

        int tile = GetTile(x, y, SetLocation::MAIN);

        if(tile == -1 && set_mode == SetMode::ONLY_BLOCK){
            return false;
        }
        if(tile != -1 && set_mode == SetMode::ONLY_AIR){
            return false;
        }     
    }

    sf::Vector2i pos = OffsetFromCoord(x, y, chunk.x, chunk.y);

    if(create_pickup){
        sf::Vector2f world_pos = CoordToWorld(x,y);

        signed_byte tile_index = chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, set_location);

        if(tile_index != -1){
            
            ItemCode item_to_drop;

            switch(set_location){
                case SetLocation::MAIN: {
                    item_to_drop = ItemDictionary::MAIN_BLOCK_DATA[tile_index].pickup;
                    break;
                }
                case SetLocation::FOREGROUND : {
                    item_to_drop = ItemDictionary::FOREGROUND_BLOCK_DATA[tile_index].pickup;                
                    break;
                }
                case SetLocation::BACKGROUND : {
                    item_to_drop = ItemDictionary::BACKGROUND_BLOCK_DATA[tile_index].pickup;
                    break;
                }
            }

            CreatePickup(item_to_drop, world_pos.x + ItemDictionary::half_tile_size, world_pos.y + ItemDictionary::half_tile_size);
        }

    }

    signed_byte old_tile = chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, set_location);

    chunks.at(chunk.x).at(chunk.y)->SetTile(tile_index, pos.x, pos.y, set_location);
    chunks[chunk.x][chunk.y]->dirty = true; // marking the chunk as "dirty" (changed)


    if(propogate_to_other_tiles){

        // communicate for water manager
        chunks.at(chunk.x).at(chunk.y)->SetAwakeForWaterSim(true);
        if(chunks.at(chunk.x).at(chunk.y)->water_updated.size() > 0){
            chunks.at(chunk.x).at(chunk.y)->water_updated[pos.x][pos.y] = true;
        }

        TileBehaviourManager::PropogateTile(x, y, tile_index, old_tile, set_location);
    
        if(set_location == SetLocation::MAIN || set_location == SetLocation::BACKGROUND){
            chunks.at(chunk.x).at(chunk.y)->CalculateSkyLight();
            /*
            // solid
            if(tile_index != -1){
                // if we create a create a pickup the block has been hand broken, 
                //we must tell the already calculated sky light of our changes
                chunks.at(chunk.x).at(chunk.y)->IntroduceTileToSkylight(pos.x, pos.y);

            }
            else{
                chunks.at(chunk.x).at(chunk.y)->RemoveTileFromSkylight(pos.x, pos.y);

            }
            */
        }
    }


    DrawTileToMinimap(tile_index, x, y, set_location);

    if(send_packet){
        client->SendSetBlock(tile_index, x, y, set_location);
    }

    world_needs_pathfinding_recalculating = true;

    return true;
}

void World::DrawTileToMinimap(signed_byte tile_index, int x, int y, SetLocation set_location){

    switch(set_location){

        case SetLocation::MAIN:
            
            if(tile_index == -1){                      
                minimap->GetMainPixelGrid()->SetPixel(x, y,  sf::Color::Transparent);
            }
            else{
                minimap->GetMainPixelGrid()->SetPixel(x, y, ItemDictionary::MAIN_BLOCK_DATA[tile_index].base_colour);
            }
            break;
        case SetLocation::BACKGROUND:
            
            if(tile_index == -1){                           // a nice sky colour
                minimap->GetBackgroundPixelGrid()->SetPixel(x, y,  sf::Color(130, 152, 150));
            }
            else{
                minimap->GetBackgroundPixelGrid()->SetPixel(x, y, ItemDictionary::BACKGROUND_BLOCK_DATA[tile_index].base_colour);
            }
            break;
        case SetLocation::FOREGROUND:
            
            if(tile_index == -1){                          
                minimap->GetForegroundPixelGrid()->SetPixel(x, y,  sf::Color::Transparent);
            }
            else{
                minimap->GetForegroundPixelGrid()->SetPixel(x, y, ItemDictionary::FOREGROUND_BLOCK_DATA[tile_index].base_colour);
            }
            break;

    }
}

void World::CreatePickup(ItemCode item_to_drop, float world_x, float world_y){

    if(item_to_drop == item_NO_DROP){
        return;
    }

    int rand_offset_x = -2 + rand() % 4;
    int rand_offset_y = -2 + rand() % 4;


    Pickup* pickup = GetScene()->AddObject<Pickup>();
    pickup->SetItemCode(item_to_drop);
    pickup->GetTransform()->position = sf::Vector2f(world_x + rand_offset_x, world_y + rand_offset_y);
    pickup->AttractToTransform(focus);
    client->GetInventory()->PickupItem(item_to_drop);

}
    
signed_byte World::GetTileWorld(float world_x, float world_y, SetLocation get_location){
    sf::Vector2i coord = WorldToCoord(world_x, world_y);
    return GetTile(coord.x, coord.y, get_location);
}

signed_byte World::GetTile(int x, int y, SetLocation get_location){
    
    sf::Vector2i chunk = ChunkFromCoord(x, y);
    if(!ChunkInBounds(chunk.x, chunk.y)){
        return -1;
    }

    sf::Vector2i pos = OffsetFromCoord(x, y, chunk.x, chunk.y);
    return chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, get_location); 
}

EntireTile World::GetEntireTile(int x, int y){
    
    sf::Vector2i chunk = ChunkFromCoord(x, y);
    if(!ChunkInBounds(chunk.x, chunk.y)){
        return {-1,-1,-1};
    }

    sf::Vector2i pos = OffsetFromCoord(x, y, chunk.x, chunk.y);
    return { chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, SetLocation::MAIN), 
            chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, SetLocation::BACKGROUND),
            chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, SetLocation::FOREGROUND)};
}

sf::Vector2i World::GetNearestTileInDirectionOfMouse(sf::Vector2f world_position, SetLocation set_location){

    sf::Vector2f mouse_world_pos = Camera::ScreenToWorldPosition(Mouse::DisplayPosition());
    sf::Vector2f direction = Calc::Normalize(mouse_world_pos - world_position);

    for(int i = 0; i < 5; i++){
        
        world_position += direction * 4.0f;

        sf::Vector2i rounded_world = RoundWorld(world_position.x, world_position.y);
        sf::Vector2i coord = WorldToCoord(rounded_world.x, rounded_world.y);

        std::vector<sf::Vector2i> tiles_in_radius = GetOffsetsInRadius(2);

        for(auto tile : tiles_in_radius){

            signed_byte _found_tile = GetTile(coord.x + tile.x, coord.y + tile.y, set_location);
            if(_found_tile != -1){
                return sf::Vector2i(coord.x + tile.x, coord.y + tile.y);
            }
        }
    }
    return sf::Vector2i(-1,-1);
}

bool World::ChunkInBounds(int chunk_x, int chunk_y){
    if(chunk_x < 0 || chunk_y < 0 || chunk_x >= world_profile.width || chunk_y >= world_profile.height){
        return false;
    }
    return true;
}
bool World::CoordInBounds(int x, int y){
    if(x < 0 || x >= world_profile.width_in_tiles || y < 0 || y >= world_profile.height_in_tiles){
        return false;
    }
    return true;
}

sf::Vector2i World::RoundWorld(float world_x, float world_y){
    sf::Vector2i world;

    world.x = round((float)(world_x - 4) / tilemap_profile->tile_width) * tilemap_profile->tile_width;
    world.y = round((float)(world_y - 4) / tilemap_profile->tile_height) * tilemap_profile->tile_height;

    return world;
}

sf::Vector2i World::ChunkFromCoord(int x, int y){
    sf::Vector2i chunk;

    chunk.x = floor((float)x * one_divide_tilemap_width);
    chunk.y = floor((float)y * one_divide_tilemap_height);

    return chunk;
}

sf::Vector2i World::OffsetFromCoord(int x, int y, int chunk_x, int chunk_y){
    
    sf::Vector2i pos;

    pos.x = x - (chunk_x * tilemap_profile->width);
    pos.y = y - (chunk_y * tilemap_profile->height);
    
    return pos;
}

sf::Vector2i World::WorldToCoord(float world_x, float world_y){
    
    sf::Vector2i coord;
    
    coord.x = round((world_x - tilemap_profile->tile_width / 2.0f) / (float)tilemap_profile->tile_width);
    coord.y = round((world_y - tilemap_profile->tile_height / 2.0f) / (float)tilemap_profile->tile_height);

    return coord;
}

sf::Vector2f World::CoordToWorld(int x, int y){
    
    sf::Vector2f world;
    
    world.x = round(x * tilemap_profile->tile_width);
    world.y = round(y * tilemap_profile->tile_height);

    return world;
}

std::vector<sf::Vector2i> World::GetOffsetsInRadius(int radius){
    
    if(radius_offsets.find(radius) == radius_offsets.end()){
        radius_offsets[radius] = CalculateOffsetsInRadius(radius);
    }

    return radius_offsets[radius];
}

std::vector<sf::Vector2i> World::CalculateOffsetsInRadius(int radius){

    std::vector<sf::Vector2i> in_radius;

    for(int x = -radius; x <= radius; x++) {
        
        int ydist = sqrt(radius * radius - x * x);

        for(int y = -ydist; y <= ydist; y++) {

            in_radius.push_back(sf::Vector2i(x, y));
        }
    }
    return in_radius;
}

void World::SetCircle(signed_byte tile_index, int x, int y, int radius, SetLocation set_location, SetMode set_mode){

    // set all tiles
    for(auto& offset : GetOffsetsInRadius(radius)){

        // converting offset to world position
        int _x = offset.x + x;
        int _y = offset.y + y;

        SetTile(tile_index, _x, _y, set_location, set_mode);
    }
}

void World::SetFocus(Transform* focus){
    this->focus = focus;
}

void World::Update(){
    
    if(!world_has_been_created){
        return;
    }


    if(focus == nullptr){
        std::cout << "ERROR : The world has no focus, use World::SetFocus()\n";
        return;
    }
    

    std::vector<Object*>* objects_additional = GetScene()->GetThisObjectsAdditional();
    std::vector<Object*>* ui_additional = GetScene()->GetUIAdditional();
    objects_additional->clear();
    ui_additional->clear();
    
    sf::Vector2i wrld_to_coord = WorldToCoord(focus->position.x, focus->position.y);
    sf::Vector2i chunk_focus_is_in = ChunkFromCoord(wrld_to_coord.x, wrld_to_coord.y);


    active_chunks_min.x = Calc::Clamp(chunk_focus_is_in.x - load_distance, 0, world_profile.width);
    active_chunks_min.y = Calc::Clamp(chunk_focus_is_in.y - load_distance, 0, world_profile.height);
    active_chunks_max.x = Calc::Clamp(chunk_focus_is_in.x + load_distance, 0, world_profile.width);
    active_chunks_max.y = Calc::Clamp(chunk_focus_is_in.y + load_distance, 0, world_profile.height);

    bool change_made = false;

    for(int x = 0; x < world_profile.width; x++){
        for(int y = 0; y < world_profile.height; y++){
            
            if(x > chunk_focus_is_in.x - load_distance && x < chunk_focus_is_in.x + load_distance){
                if(y > chunk_focus_is_in.y - load_distance && y < chunk_focus_is_in.y + load_distance){

                    // when a chunk initally loads in, we recalculate colliders no matter what
                    if(!chunks[x][y]->loaded_in_scene){
                        change_made = true;
                        chunks[x][y]->ResetColliders();
                    }
                    else{ // only recalculating colliders if a change is made to said chunk
                        chunks[x][y]->ResetCollidersIfChanged();
                    }

                    chunks[x][y]->loaded_in_scene = true;

                    // adding objects found in chunk
                    for(int i = 0; i < chunks[x][y]->GetThisObjectsInChunk()->size(); i++){
                        objects_additional->push_back((*chunks[x][y]->GetThisObjectsInChunk())[i]);
                    }
                    for(int i = 0; i < chunks[x][y]->GetUIInChunk()->size(); i++){
                        ui_additional->push_back((*chunks[x][y]->GetUIInChunk())[i]);
                    }

                    objects_additional->push_back(chunks[x][y]);
                    chunks[x][y]->SetActive(true);
                    
                    continue;
                }
            }
            chunks[x][y]->SetActive(false);

            if(chunks[x][y]->loaded_in_scene){
                change_made = true;
            }

            chunks[x][y]->ClearColliders();
            chunks[x][y]->loaded_in_scene = false;
        }

        if(change_made){
            world_needs_pathfinding_recalculating = true;
        }
    }


    LightingManager::DrawLightSources();
    LightingManager::ResetLightDelay();

    TimeManager::Tick();

    TileBehaviourManager::PerformTileUpdatePass();
    

    RevealMapAroundFocus();
}

void World::RevealMapAroundFocus(){


    sf::Vector2i pos = WorldToCoord(focus->position.x, focus->position.y);

    int lightmap_max;
    sf::Vector2i chunk_coord;
    sf::Vector2i chunk_offset;

    // iterates over all positions in a specific radius (to explore on map)
    for(auto offset : GetOffsetsInRadius(Minimap::exploring_radius)){
        
        sf::Vector2i coord = offset + pos;

        if(CoordInBounds(coord.x, coord.y)){
            
            chunk_coord = ChunkFromCoord(coord.x, coord.y);
            chunk_offset = OffsetFromCoord(coord.x, coord.y, chunk_coord.x, chunk_coord.y);

            // is the chunk active? if not the lightmap wont be valid
            if(chunks.at(chunk_coord.x).at(chunk_coord.y)->IsActive()){

                sf::Image& lightmap = chunks.at(chunk_coord.x).at(chunk_coord.y)->GetLightmap();

                if(lightmap.getSize().x < tilemap_profile->width){
                    continue;
                }

                // find max colour attribute, from either dynamic lightmap or skylight, whatever is larger
                lightmap_max = std::max(std::max(lightmap.getPixel(chunk_offset.x, chunk_offset.y).r,
                                            lightmap.getPixel(chunk_offset.x, chunk_offset.y).g),
                                            lightmap.getPixel(chunk_offset.x, chunk_offset.y).b);
                

                // only draw if the new colour is lighter (keeping exploration peristant)
                if(255 - lightmap_max < minimap->GetExploredPixelGrid()->GetPixel(coord.x, coord.y).a){
                    minimap->GetExploredPixelGrid()->SetPixel(coord.x, coord.y, sf::Color(0, 0, 0, 255 - lightmap_max));
                }

            }

        }
    }

}

bool World::CoordIsConnectedToOtherTiles(int x, int y){

    sf::Vector2i offsets_to_check[] = {sf::Vector2i(0,0), sf::Vector2i(0,1), sf::Vector2i(1,0), sf::Vector2i(-1,0), sf::Vector2i(0,-1)};

    for(int i = 0; i < 5; i++){
        if(GetTile(x + offsets_to_check[i].x, y + offsets_to_check[i].y, SetLocation::BACKGROUND) != -1){
            return true;
        }
        if(GetTile(x + offsets_to_check[i].x, y + offsets_to_check[i].y, SetLocation::MAIN) != -1){
            return true;
        }
    }
}


void World::SetWorldNeedsPathfindingRecalculating(bool state){
    world_needs_pathfinding_recalculating = state;
}

void World::SetSpawnCoordinate(int coord_x, int coord_y){
    this->world_spawn_coord.x = coord_x;
    this->world_spawn_coord.y = coord_y;
}

World::~World(){
    for(int x = 0; x < world_profile.width; x++){
        for(int y = 0; y < world_profile.height; y++){
            Memory::Delete<Chunk>(chunks[x][y], __FUNCTION__);
        }
    }
}
