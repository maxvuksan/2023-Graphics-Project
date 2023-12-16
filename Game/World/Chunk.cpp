#include "Chunk.h"
#include "LightingManager.h"
#include "World.h"

void Chunk::Init(World* world){

    this->world = world;

    SetRenderLayer(10);

    main_tilemap = this->AddComponent<Tilemap>(0);
    foreground_tilemap = this->AddComponent<Tilemap>(0);
    background_tilemap = this->AddComponent<Tilemap>(-1);

    foreground_tilemap->Load("foreground_tiles", &world->world_profile.tilemap_profile);
    main_tilemap->Load("tiles", &world->world_profile.tilemap_profile);
    background_tilemap->Load("background_tiles", &world->world_profile.tilemap_profile);
    
    skylight_map.resize(world->world_profile.tilemap_profile.width);
    for(int i = 0; i < skylight_map.size(); i++){
        skylight_map[i].resize(world->world_profile.tilemap_profile.height, 0);
    }
    
    // assigning a collider to the main tilemap
    auto collider = this->AddComponent<TilemapCollider>();
    collider->SetTilemap(main_tilemap);
    
    skylight_map_dirty = true;
    light_map_dirty = false;

    skylight_first_calculated = false;
}

void Chunk::Draw(sf::RenderTarget& surface){

    if(light_map_dirty){
        light_map_dirty = false;    
        light_texture.loadFromImage(light_map);
    }

    if(skylight_map_dirty){
        skylight_map_dirty = false;
        skylight_texture.loadFromImage(skylight_image);
    }
}

Tilemap* Chunk::GetTilemap(SetLocation set_location){

    if(set_location == SetLocation::MAIN){
        return main_tilemap;
    }
    else if(set_location == SetLocation::BACKGROUND){
        return background_tilemap;
    }
    else{
        return foreground_tilemap;
    }
}

int Chunk::GetTile(int x, int y, SetLocation get_location){
    
    if(get_location == SetLocation::MAIN){
        return main_tilemap->GetTile(x, y);
    }
    else if(get_location == SetLocation::BACKGROUND){
        return background_tilemap->GetTile(x, y);
    }
    else{
        return foreground_tilemap->GetTile(x, y);
    }
}

void Chunk::SetTile(int tile_index, int x, int y, SetLocation set_location){

    if(set_location == SetLocation::MAIN){
        main_tilemap->SetTileSafe(tile_index, x, y);
    }
    else if(set_location == SetLocation::BACKGROUND){
        background_tilemap->SetTileSafe(tile_index, x, y);
    }
    else{
        foreground_tilemap->SetTileSafe(tile_index, x, y);
    }
}

void Chunk::ClearLightmap(){

    light_map.create(world->world_profile.tilemap_profile.width, world->world_profile.tilemap_profile.height, sf::Color(0, 0, 0));

}

void Chunk::CalculateSkyLight(){
    
    skylight_first_calculated = true;
    
    sf::Vector2i offsets[4] = { sf::Vector2i(1,0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1)};
    
    for(unsigned int x = 0; x < world->world_profile.tilemap_profile.width; x++){
        for(unsigned int y = 0; y < world->world_profile.tilemap_profile.height; y++){
            
            if(background_tilemap->GetTile(x, y) == -1 && main_tilemap->GetTile(x,y) == -1){
                skylight_map[x][y] = 255;
            }
            else{
                continue;
            }

            for(auto offset : offsets){
                sf::Vector2i coord = sf::Vector2i(x,y) + world->WorldToCoord(GetTransform()->position.x, GetTransform()->position.y) + offset;

                if(world->GetTile(coord.x, coord.y) != -1 || world->GetTile(coord.x, coord.y, SetLocation::BACKGROUND) != -1){
                    
                    LightingManager::PropogateSkyLighting(coord, 255);
                }
            }
        }
    }    
}

void Chunk::RefreshSkylight(){
    skylight_image.create(world->world_profile.tilemap_profile.width, world->world_profile.tilemap_profile.height, sf::Color(0, 0, 0));

    if(!skylight_first_calculated){
        CalculateSkyLight();
    }

    for(unsigned int x = 0; x < world->world_profile.tilemap_profile.width; x++){
        for(unsigned int y = 0; y < world->world_profile.tilemap_profile.height; y++){

            skylight_image.setPixel(x, y, sf::Color(skylight_map[x][y], skylight_map[x][y], skylight_map[x][y]));

        }
    }
}

void Chunk::OnSetActive(){
    
    RefreshSkylight();
    ClearLightmap();
    
    light_map_dirty = true;

    main_tilemap->ConstructVertexArray();
    foreground_tilemap->ConstructVertexArray();
    background_tilemap->ConstructVertexArray();
}

void Chunk::OnDisable(){
    light_map.create(1,1);
    light_texture.create(1,1);
    skylight_image.create(1,1);
    main_tilemap->ClearVertexArray();
    foreground_tilemap->ClearVertexArray();
    background_tilemap->ClearVertexArray();
}

void Chunk::ClearColliders(){
    GetComponent<TilemapCollider>()->ClearRects();
}
void Chunk::ResetCollidersIfChanged(){
    if(dirty){
        GetComponent<TilemapCollider>()->Reset();
        dirty = false;
    }
}
void Chunk::ResetColliders(){
    GetComponent<TilemapCollider>()->Reset();
}

Chunk::~Chunk(){
    for(auto obj : objects_bound_to_chunk){
        Memory::Delete<Object>(obj, __FUNCTION__);
    }
    for(auto obj : ui_bound_to_chunk){
        Memory::Delete<Object>(obj, __FUNCTION__);
    }
}