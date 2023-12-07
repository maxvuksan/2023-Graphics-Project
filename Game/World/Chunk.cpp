#include "Chunk.h"

void Chunk::Init(TilemapProfile* _tilemap_profile, const sf::Color& background_colour){

    SetRenderLayer(10);

    this->tilemap_profile = _tilemap_profile;

    main_tilemap = this->AddComponent<Tilemap>(0);
    foreground_tilemap = this->AddComponent<Tilemap>(0);
    background_tilemap = this->AddComponent<Tilemap>(-1);

    foreground_tilemap->SetInteractsLight(false);
    background_tilemap->SetInteractsLight(false);
    //background_tilemap->SetShowOverlayColour(true);
    //background_tilemap->SetOverlayColour(background_colour);

    foreground_tilemap->Load("foreground_tiles", tilemap_profile);
    main_tilemap->Load("tiles", tilemap_profile);
    background_tilemap->Load("background_tiles", tilemap_profile);
    
    
    // assigning a collider to the main tilemap
    auto collider = this->AddComponent<TilemapCollider>();
    collider->SetTilemap(main_tilemap);
}

void Chunk::Draw(sf::RenderTarget& surface){

    if(light_map_dirty){
        light_map_dirty = false;
        light_texture.loadFromImage(light_map);
    }

    sf::Sprite light_sprite(light_texture);
    light_sprite.setScale(sf::Vector2f(tilemap_profile->tile_width, tilemap_profile->tile_height));
    light_sprite.setPosition(Camera::WorldToScreenPosition(GetTransform()->position));    
    surface.draw(light_sprite, sf::BlendMultiply);
    
    light_map.create(tilemap_profile->width, tilemap_profile->height, sf::Color(0,0,0));
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

void Chunk::OnSetActive(){

    light_map.create(tilemap_profile->width, tilemap_profile->height, sf::Color(0,0,0));
    light_map_dirty = true;

    main_tilemap->ConstructVertexArray();
    foreground_tilemap->ConstructVertexArray();
    background_tilemap->ConstructVertexArray();
}

void Chunk::OnDisable(){
    light_map.create(1,1);
    light_texture.create(1,1);

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