#include "Chunk.h"

void Chunk::Init(TilemapProfile* tilemap_profile, const sf::Color& background_colour){

    foreground_tilemap_object = AddChild<Object>(0);
    background_tilemap_object = AddChild<Object>(-1);
    
    foreground_tilemap = foreground_tilemap_object->AddComponent<Tilemap>();
    background_tilemap = background_tilemap_object->AddComponent<Tilemap>();
    background_tilemap->SetInteractsLight(false);
    background_tilemap->SetShowOverlayColour(true);
    background_tilemap->SetOverlayColour(background_colour);

    foreground_tilemap->Load("demoTexture", tilemap_profile);
    background_tilemap->Load("demoTexture", tilemap_profile);
    
    // assigning a collider to the main tilemap
    auto collider = foreground_tilemap_object->AddComponent<TilemapCollider>();
    collider->SetTilemap(foreground_tilemap);
}

Tilemap* Chunk::GetTilemap(SetLocation set_location){

    if(set_location == SetLocation::FOREGROUND){
        return foreground_tilemap;
    }
    else{
        return background_tilemap;
    }
}

int Chunk::GetTile(int x, int y, SetLocation get_location){
    
    if(get_location == SetLocation::FOREGROUND){
        return foreground_tilemap->GetTile(x, y);
    }
    else{
        return background_tilemap->GetTile(x, y);
    }
}

void Chunk::SetTile(int tile_index, int x, int y, SetLocation set_location){

    if(set_location == SetLocation::FOREGROUND){
        foreground_tilemap->SetTileSafe(tile_index, x, y);
    }
    else{
        background_tilemap->SetTileSafe(tile_index, x, y);
    }
}

void Chunk::ClearColliders(){
    foreground_tilemap_object->GetComponent<TilemapCollider>()->ClearRects();
}
void Chunk::EnableColliders(){
    foreground_tilemap_object->GetComponent<TilemapCollider>()->Reset();
}
