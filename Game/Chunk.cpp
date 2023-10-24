#include "Chunk.h"

void Chunk::Init(int tilemap_width, int tilemap_height, int tilesize_x, int tilesize_y){
    this->tilemap_width = tilemap_width;
    this->tilemap_height = tilemap_height;
    this->tilesize_x = tilesize_x;
    this->tilesize_y = tilesize_y;

    main_tilemap = AddComponent<Tilemap>();
    background_tilemap = AddComponent<Tilemap>();

    main_tilemap->Load("demoTexture", tilesize_x, tilesize_y, tilemap_width, tilemap_height);
    background_tilemap->Load("demoTexture", tilesize_x, tilesize_y, tilemap_width, tilemap_height);
    
    // assigning a collider to the main tilemap
    auto collider = AddComponent<TilemapCollider>();
    collider->SetTilemap(main_tilemap);
}
