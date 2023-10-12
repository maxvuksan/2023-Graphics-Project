#include "TilemapPrimitive.h"
#include "../../Core/Globals.h"
#include <iostream>

std::vector<std::vector<int>>& TilemapPrimitive::GetGrid(){
    return grid;
}

bool TilemapPrimitive::Load(sf::Texture* texture, sf::Vector2u tile_size, unsigned int width, unsigned int height, int default_tile){
    
    grid.clear();
    grid.resize(width);
    
    this->tile_size = tile_size;
    this->size = sf::Vector2u(width, height);

    texture_reference = texture;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < width; ++i){
        
        grid[i].resize(height);
        
        for (unsigned int j = 0; j < height; ++j)
        {

            SetTile(default_tile, i, j);
        }
    }

    return true;
}

void TilemapPrimitive::RevertTexture(){
    focus_texture = texture_reference;
}
void TilemapPrimitive::SetTexture(sf::Texture* texture){
    focus_texture = texture;
}

void TilemapPrimitive::SetTile(int tile_index, unsigned int x, unsigned int y){

    grid[x][y] = tile_index;

    // find its position in the tileset texture
    int tu, tv;
    
    if(tile_index >= 0){
        tu = tile_index % (texture_reference->getSize().x / tile_size.x);
        tv = tile_index / (texture_reference->getSize().x / tile_size.x);
    }

    // get a pointer to the triangles' vertices of the current tile
    sf::Vertex* triangles = &m_vertices[(x + y * size.x) * 6];

    // define the 6 corners of the two triangles
    if(tile_index >= 0){
        triangles[0].position = sf::Vector2f(x * tile_size.x, y * tile_size.y);
        triangles[1].position = sf::Vector2f((x + 1) * tile_size.x, y * tile_size.y);
        triangles[2].position = sf::Vector2f(x * tile_size.x, (y + 1) * tile_size.y);
        triangles[3].position = sf::Vector2f(x * tile_size.x, (y + 1) * tile_size.y);
        triangles[4].position = sf::Vector2f((x + 1) * tile_size.x, y * tile_size.y);
        triangles[5].position = sf::Vector2f((x + 1) * tile_size.x, (y + 1) * tile_size.y);

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
    triangles[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
    triangles[1].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
    triangles[2].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
    triangles[3].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
    triangles[4].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
    triangles[5].texCoords = sf::Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
}