#include "Chunk.h"

void Chunk::Init(int tilemap_width, int tilemap_height, int tilesize_x, int tilesize_y, const sf::Color& background_colour){
    this->tilemap_width = tilemap_width;
    this->tilemap_height = tilemap_height;
    this->tilesize_x = tilesize_x;
    this->tilesize_y = tilesize_y;


    /*
        splitting the foreground tilemap and background tilemap to seperate objects, 
        this will allow me to set different render layers,

        should be added with AddChild<>

        should implement a parent -> child active state,
        
        when a parent is not active, disable the child.
       
    
    */

    foreground_tilemap_object = AddChild<Object>(0);
    background_tilemap_object = AddChild<Object>(-1);
    
    foreground_tilemap = foreground_tilemap_object->AddComponent<Tilemap>();
    background_tilemap = background_tilemap_object->AddComponent<Tilemap>();
    background_tilemap->SetInteractsLight(false);
    background_tilemap->SetShowOverlayColour(true);
    background_tilemap->SetOverlayColour(background_colour);

    foreground_tilemap->Load("demoTexture", tilesize_x, tilesize_y, tilemap_width, tilemap_height);
    background_tilemap->Load("demoTexture", tilesize_x, tilesize_y, tilemap_width, tilemap_height);
    
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
