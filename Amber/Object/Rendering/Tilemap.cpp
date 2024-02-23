#include "Tilemap.h"
#include "../Object.h"
#include "../../Core/Core.h"
#include "../../Core/Scene.h"
#include "../Camera.h"
#include <math.h>
#include <array>


void Tilemap::Start(){
    
    show_overlay_colour = false;
    overlay_colour = sf::Color(0,0,0,0.5);

}

Tilemap::~Tilemap(){

}

void Tilemap::UpdateSecondary(){
    
    tilemap_primitive.setPosition(Camera::WorldToScreenPosition(
        object->GetTransform()->GetGlobalPosition()
    ));

    tilemap_primitive.setScale(object->GetTransform()->scale);
    // rotation should have no effect on a tilemap (we cannot rotate)

    has_changed = false; // reset changed value
}

void Tilemap::ConstructVertexArray(){
    if(loaded){
        tilemap_primitive.ConstructVertexArray();
    }
}


void Tilemap::ClearVertexArray(){
    tilemap_primitive.ClearVertexArray();
}


signed_byte Tilemap::GetTile(int x, int y){
    std::array<std::array<signed_byte, TILEMAP_HEIGHT>, TILEMAP_WIDTH>& grid = tilemap_primitive.GetGrid();

    if(x < tilemap_profile->width && x >= 0){
        if(y < tilemap_profile->height && y >= 0){
            return grid.at(x).at(y);
        }
    }
    return -1;
}

void Tilemap::Draw(sf::RenderTarget& surface){

    if(!this->loaded){
        std::cout << "ERROR : Attempting to draw tilemap which has not been loaded, Tilemap::Draw()\n";
        return;
    }

    tilemap_primitive.RevertTexture();
    surface.draw(tilemap_primitive);

    if(show_overlay_colour){

        //std::cout << overlay_colour.r << " " << overlay_colour.g << " " << overlay_colour.b << " " << overlay_colour.a << "\n";

        overlay_shader->setParameter("u_colour", sf::Color(overlay_colour.r, overlay_colour.g, overlay_colour.b));
        overlay_shader->setParameter("u_blend", overlay_colour.a / 255.0f);
        surface.draw(tilemap_primitive, overlay_shader);
    }
}

void Tilemap::DrawSiloutte(sf::RenderTarget& surface){
    
    tilemap_primitive.RevertTexture();
    surface.draw(tilemap_primitive, AssetManager::GetShader("Amber_ColourToBlack"));
}
void Tilemap::DrawWithExternalTexture(sf::RenderTarget& surface, sf::Texture* texture){


    tilemap_primitive.SetTexture(texture);
    surface.draw(tilemap_primitive);
    tilemap_primitive.RevertTexture();
}

bool Tilemap::Load(const char* texture_label, TilemapProfile* profile, int default_tile){
    
        
    bool state = tilemap_primitive.Load(AssetManager::GetTexture(texture_label), profile, default_tile);
    
    if(state == false){
        std::cout << "ERROR : Failed to call TilemapPrimitive::Load()\n";
        return false;
    }

    tilemap_profile = profile;

    overlay_shader = AssetManager::GetShader("Amber_ColourOverlay");

    loaded = true;
    has_changed = true;
    return true;
}



void Tilemap::SetTile(signed_byte tile_index, unsigned int x, unsigned int y){
    tilemap_primitive.SetTile(tile_index, x, y);
    has_changed = true;
}
bool Tilemap::SetTileSafe(signed_byte tile_index, int x, int y){

    if(x >= 0 && x < tilemap_profile->width){
        if(y >= 0 && y < tilemap_profile->height){
            tilemap_primitive.SetTile(tile_index, x, y);
            has_changed = true;
            return true;
        }
    }
    return false;
}

void Tilemap::SetRow(signed_byte tile_index, unsigned int row){    
    for(unsigned int x = 0; x < tilemap_profile->width; x++){
        tilemap_primitive.SetTile(tile_index, x, row);
    }
}
void Tilemap::SetColumn(signed_byte tile_index, unsigned int column){
    for(unsigned int y = 0; y < tilemap_profile->height; y++){
        tilemap_primitive.SetTile(tile_index, column, y);
    }
}
void Tilemap::SetArea(signed_byte tile_index, unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max){
    for(unsigned int x = x_min; x < x_max; x++){
        for(unsigned int y = y_min; y < y_max; y++){
            tilemap_primitive.SetTile(tile_index, x, y);            
        }
    }
}

void Tilemap::SetAreaSafe(signed_byte tile_index, unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max){
    for(unsigned int x = x_min; x < x_max; x++){
        for(unsigned int y = y_min; y < y_max; y++){

            SetTileSafe(tile_index, x, y);         
        }
    }
}


void Tilemap::SetAll(signed_byte tile_index){
    for(unsigned int y = 0; y < tilemap_profile->height; y++){
        for(unsigned int x = 0; x < tilemap_profile->width; x++){
            tilemap_primitive.SetTile(tile_index, x, y);
        }
    }
}


void Tilemap::SetShowOverlayColour(bool show_overlay_colour){
    this->show_overlay_colour = show_overlay_colour;
}
void Tilemap::SetOverlayColour(sf::Color _overlay_colour){
    this->overlay_colour = _overlay_colour;
}

int Tilemap::GetWidth(){
    return tilemap_profile->width;
}
int Tilemap::GetHeight(){
    return tilemap_profile->height;
}
sf::Vector2i Tilemap::GetTileSize(){
    return sf::Vector2i(tilemap_profile->tile_width, tilemap_profile->tile_height);
}