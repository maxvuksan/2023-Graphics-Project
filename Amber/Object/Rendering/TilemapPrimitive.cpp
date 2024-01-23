#include "TilemapPrimitive.h"
#include "../../Core/Globals.h"
#include <iostream>

std::array<std::array<signed_byte, TILEMAP_HEIGHT>, TILEMAP_WIDTH>& TilemapPrimitive::GetGrid(){
    return grid;
}

bool TilemapPrimitive::Load(sf::Texture* texture, TilemapProfile* tilemap_profile, int default_tile){
    
    this->tilemap_profile = tilemap_profile;

    m_vertices.setPrimitiveType(sf::Triangles);
    
    texture_reference = texture;

    // populate the vertex array, with two triangles per tile
    for (unsigned int x = 0; x < tilemap_profile->width; ++x){
        for (unsigned int y = 0; y < tilemap_profile->height; ++y)
        {
            SetTile(default_tile, x, y);
        }
    }
    return true;
}

void TilemapPrimitive::ConstructVertexArray(){

    m_vertices.clear();
    m_vertices.resize(tilemap_profile->width * tilemap_profile->height * 6);

    // populate the vertex array, with two triangles per tile
    for (unsigned int x = 0; x < tilemap_profile->width; ++x){
        for (unsigned int y = 0; y < tilemap_profile->height; ++y)
        {
            SetTileInVertexArray(grid[x][y], x, y);
        }
    }
}

void TilemapPrimitive::ClearVertexArray(){
    m_vertices.clear();
}

void TilemapPrimitive::RevertTexture(){
    focus_texture = texture_reference;
}
void TilemapPrimitive::SetTexture(sf::Texture* texture){
    focus_texture = texture;
}

void TilemapPrimitive::SetTile(signed_byte tile_index, unsigned int x, unsigned int y){
    grid[x][y] = tile_index;
    SetTileInVertexArray(tile_index, x, y);
}

void TilemapPrimitive::SetTileInVertexArray(signed_byte tile_index, unsigned int x, unsigned int y){

    // the TilemapPrimitive vertex array is not constructed (likley because the parent object is not active)
    if(m_vertices.getVertexCount() == 0){
        return;
    }

    // find its position in the tileset texture
    int tu, tv;
    
    if(tile_index >= 0){
        tu = tile_index % (texture_reference->getSize().x / tilemap_profile->tile_width);
        tv = tile_index / (texture_reference->getSize().x / tilemap_profile->tile_width);
    }

    // get a pointer to the triangles' vertices of the current tile
    sf::Vertex* triangles = &m_vertices[(x + y * tilemap_profile->width) * 6];

    // define the 6 corners of the two triangles
    if(tile_index >= 0){
        triangles[0].position = sf::Vector2f(x * tilemap_profile->tile_width, y * tilemap_profile->tile_height);
        triangles[1].position = sf::Vector2f((x + 1) * tilemap_profile->tile_width, y * tilemap_profile->tile_height);
        triangles[2].position = sf::Vector2f(x * tilemap_profile->tile_width, (y + 1) * tilemap_profile->tile_height);
        triangles[3].position = sf::Vector2f(x * tilemap_profile->tile_width, (y + 1) * tilemap_profile->tile_height);
        triangles[4].position = sf::Vector2f((x + 1) * tilemap_profile->tile_width, y * tilemap_profile->tile_height);
        triangles[5].position = sf::Vector2f((x + 1) * tilemap_profile->tile_width, (y + 1) * tilemap_profile->tile_height);

    }
    else {
        triangles[0].position = sf::Vector2f(0,0);
        triangles[1].position = sf::Vector2f(0,0);
        triangles[2].position = sf::Vector2f(0,0);
        triangles[3].position = sf::Vector2f(0,0);
        triangles[4].position = sf::Vector2f(0,0);
        triangles[5].position = sf::Vector2f(0,0);
    }
    // define the 6 matching texture coordinates
    triangles[0].texCoords = sf::Vector2f(tu * tilemap_profile->tile_width, tv * tilemap_profile->tile_height);
    triangles[1].texCoords = sf::Vector2f((tu + 1) * tilemap_profile->tile_width, tv * tilemap_profile->tile_height);
    triangles[2].texCoords = sf::Vector2f(tu * tilemap_profile->tile_width, (tv + 1) * tilemap_profile->tile_height);
    triangles[3].texCoords = sf::Vector2f(tu * tilemap_profile->tile_width, (tv + 1) * tilemap_profile->tile_height);
    triangles[4].texCoords = sf::Vector2f((tu + 1) * tilemap_profile->tile_width, tv * tilemap_profile->tile_height);
    triangles[5].texCoords = sf::Vector2f((tu + 1) * tilemap_profile->tile_width, (tv + 1) * tilemap_profile->tile_height);
}