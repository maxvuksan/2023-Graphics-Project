#include "World.h"
#include <cmath>
#include "../Networking/GameClient.h"
#include "../Pickup.h"
void World::Start() {

    tilemap_profile = &world_profile.tilemap_profile;
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

    minimap = GetScene()->AddUI<Minimap>();
    minimap->GetPixelGrid()->Create(world_profile.width * tilemap_profile->width, world_profile.height * tilemap_profile->width, sf::Color::Transparent);

    sf::Color wall_colour(Globals::BASE_SHADOW_COLOUR.r / 6, Globals::BASE_SHADOW_COLOUR.g / 6, Globals::BASE_SHADOW_COLOUR.b / 6, 210);

    for(int x = 0; x < world_profile.width; x++){
        for(int y = 0; y < world_profile.height; y++){

            // create chunk and place it in the correct position
            chunks[x][y] = GetScene()->AddObject<Chunk>();
            chunks[x][y]->Init(tilemap_profile, wall_colour);
            chunks[x][y]->GetTransform()->position = sf::Vector2f(x * tilemap_profile->tile_width * tilemap_profile->width, y * tilemap_profile->tile_height * tilemap_profile->height);
        }
    }

    WorldGenerator::Bind(this);
    WorldGenerator::Generate();

    CalculateMinimap();

}


void World::LinkClient(GameClient* client){
    this->client = client;
}



void World::CalculateMinimap(){
    for(int x = 0; x < world_profile.width; x++){
        for(int y = 0; y < world_profile.height; y++){

            for(int t_x = 0; t_x < tilemap_profile->width; t_x++){
                for(int t_y = 0; t_y < tilemap_profile->height; t_y++){
                    
                    int final_x = x * tilemap_profile->width + t_x;
                    int final_y = y * tilemap_profile->height + t_y;

                    int tile = chunks.at(x).at(y)->GetTile(t_x, t_y, SetLocation::FOREGROUND);

                    if(tile == -1){
                        minimap->GetPixelGrid()->SetPixel(final_x, final_y, sf::Color::Transparent);
                    }
                    else{
                       minimap->GetPixelGrid()->SetPixel(final_x, final_y, Items[tile].base_colour);
                    }
                }
            }

        }
    }
}

bool World::SetTileWorld(short tile_index, int world_x, int world_y, SetLocation set_location, bool send_packet){

    sf::Vector2i coord = WorldToCoord(world_x, world_y);
    return SetTile(tile_index, coord.x, coord.y, set_location, SetMode::OVERRIDE, send_packet);
}

bool World::SetTile(short tile_index, int x, int y, SetLocation set_location, SetMode set_mode, bool send_packet){
    
    sf::Vector2i chunk = ChunkFromCoord(x, y);
    if(!ChunkInBounds(chunk.x, chunk.y)){
        return false;
    }

    if(set_mode != SetMode::OVERRIDE){

        int tile = GetTile(x, y, set_location);

        if(tile == -1 && set_mode == SetMode::ONLY_BLOCK){
            return false;
        }
        if(tile != -1 && set_mode == SetMode::ONLY_AIR){
            return false;
        }     
    }

    sf::Vector2i pos = OffsetFromCoord(x, y, chunk.x, chunk.y);

    chunks.at(chunk.x).at(chunk.y)->SetTile(tile_index, pos.x, pos.y, set_location);



    // updating minimap... 
    if(tile_index == -1){
        minimap->GetPixelGrid()->SetPixel(x, y,  sf::Color::Transparent);
    }
    else{
        minimap->GetPixelGrid()->SetPixel(x, y, Items[tile_index].base_colour);
    }

    if(send_packet){
        client->SendSetBlock(tile_index, x, y);
    }


    return true;
}
bool World::BreakTileWorld(int world_x, int world_y, SetLocation set_location, bool send_packet){

    // get code

    ItemCode item = (ItemCode)GetTileWorld(world_x, world_y, set_location);

    // create pickup

    Pickup* pickup = GetScene()->AddObject<Pickup>();
    pickup->SetItemCode(item);
    pickup->GetTransform()->position = sf::Vector2f(world_x, world_y);
    pickup->AttractToTransform(focus);

    client->GetInventory()->PickupItem(item);

    return SetTileWorld(-1, world_x, world_y, set_location, send_packet);
}

short World::GetTileWorld(int world_x, int world_y, SetLocation get_location){
    sf::Vector2i coord = WorldToCoord(world_x, world_y);
    return GetTile(coord.x, coord.y, get_location);
}

short World::GetTile(int x, int y, SetLocation get_location){
    
    sf::Vector2i chunk = ChunkFromCoord(x, y);
    if(!ChunkInBounds(chunk.x, chunk.y)){
        return false;
    }

    sf::Vector2i pos = OffsetFromCoord(x, y, chunk.x, chunk.y);
    return chunks.at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, get_location); 
}

bool World::ChunkInBounds(int chunk_x, int chunk_y){
    if(chunk_x < 0 || chunk_y < 0 || chunk_x >= world_profile.width || chunk_y >= world_profile.height){
        return false;
    }
    return true;
}

sf::Vector2i World::RoundWorld(int world_x, int world_y){
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

sf::Vector2i World::WorldToCoord(int world_x, int world_y){
    
    sf::Vector2i coord;
    
    coord.x = round((world_x - tilemap_profile->tile_width / 2.0f) / (float)tilemap_profile->tile_width);
    coord.y = round((world_y - tilemap_profile->tile_height / 2.0f) / (float)tilemap_profile->tile_height);

    return coord;
}

sf::Vector2i World::CoordToWorld(int x, int y){
    
    sf::Vector2i world;
    
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

void World::SetCircle(short tile_index, int x, int y, int radius, SetLocation set_location, SetMode set_mode){

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
    
    if(focus == nullptr){
        std::cout << "ERROR : The world has no focus, use World::SetFocus()\n";
        return;
    }

    for(int x = 0; x < world_profile.width; x++){
        for(int y = 0; y < world_profile.height; y++){

            int real_x = x * tilemap_profile->width * tilemap_profile->tile_width;
            int real_y = y * tilemap_profile->height * tilemap_profile->tile_height;
            
            int dis_x = abs(focus->position.x - real_x);
            int dis_y = abs(focus->position.y - real_y);

            if(dis_x < collider_threshold &&
               dis_y < collider_threshold){
                
                chunks[x][y]->EnableColliders();
            }
            else{
                chunks[x][y]->ClearColliders();
            }


            if(dis_x < loading_threshold &&
               dis_y < loading_threshold){

                chunks[x][y]->SetActive(true);

            }
            else{
                chunks[x][y]->SetActive(false);
            }
        }
    }

}