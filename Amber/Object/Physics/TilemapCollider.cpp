#include "TilemapCollider.h"
#include "../Rendering/Tilemap.h"
#include "../Object.h"
#include <iostream>
#include "TilemapCollisionTypeDivider.h"

TilemapCollider::TilemapCollider(): tilemap(nullptr), reset(true){}

void TilemapCollider::Update(){

    // ensuring we have a Tilemap to work with
    if(tilemap == nullptr){
        
        std::cout << "ERROR : A TilemapCollider is attached to an Object without a Tilemap, please set a tilemap with TilemapCollider::SetTilemap(), TilemapCollider::Update()\n";
        return;
    }
    else if(reset){
        reset = false;

        ClearRects();
        CreateCollidersOptimized();
    }
}

void TilemapCollider::SetTilemap(Tilemap* tilemap){
    this->tilemap = tilemap;
}

void TilemapCollider::Reset(){
    reset = true;
}


bool TilemapCollider::WithinBoundsX(float x){

    if(tilemap == nullptr){
        return false;
    }

    sf::Vector2f t_pos = object->GetTransform()->GetGlobalPosition();

    if(x > t_pos.x && x < t_pos.x + (tilemap->GetWidth() * tilemap->GetTileSize().x)){
        return true;
    }
    return false;
}
bool TilemapCollider::WithinBoundsY(float y){

    if(tilemap == nullptr){
        return false;
    }

    sf::Vector2f t_pos = object->GetTransform()->GetGlobalPosition();

    if(y > t_pos.y && y < t_pos.y + (tilemap->GetHeight() * tilemap->GetTileSize().y)){
        return true;
    }
    return false;
}

void TilemapCollider::CreateColliders(){

    sf::Vector2i tile_size = tilemap->GetTileSize();


    for(unsigned int x = 0; x < tilemap->GetWidth(); x++){
        for(unsigned int y = 0; y < tilemap->GetHeight(); y++){

            if(tilemap->GetTile(x, y) == -1){
                continue;
            }

            ColliderType collision_mode = ColliderType::SOLID;
            if(tilemap->GetTile(x, y) >= TilemapCollisionTypeDivider::platform_collider_begins_at){
                collision_mode = ColliderType::PLATFORM;
            }
            
            AddRect(x * tile_size.x, y * tile_size.y, tile_size.x, tile_size.y, collision_mode);
        }
    }
}

void TilemapCollider::CreateCollidersOptimized(){

    sf::Vector2i tile_size = tilemap->GetTileSize();

    // will run through three passes, 
    // first horizontal joining adjacent tiles, 
    // then vertical joining rows
    // final adding the joined rows as Collider Rects

    std::vector<std::vector<Rect>> blocks; // [y][x]
    blocks.resize(tilemap->GetHeight());

    // construct rows _________________________________________
    for(unsigned int y = 0; y < tilemap->GetHeight(); y++){
        
        bool tracing_block = false;

        Rect block;
        block.y = y;
        block.height = 1;

        for(unsigned int x = 0; x < tilemap->GetWidth(); x++){


            if(tilemap->GetTile(x, y) == -1){

                block.width = x - block.x;
                
                if(tracing_block){
                    blocks.at(y).push_back(block);
                }

                block.width = 0;
                tracing_block = false;
            }
            
            else{
                if(tracing_block){
                    // solid cannot merge with platform or platform cannot merge with solid
                    if((block.collision_mode == ColliderType::SOLID && tilemap->GetTile(x, y) >= TilemapCollisionTypeDivider::platform_collider_begins_at)
                        || (block.collision_mode == ColliderType::PLATFORM && tilemap->GetTile(x, y) < TilemapCollisionTypeDivider::platform_collider_begins_at)){

                        block.width = x - block.x;
                        
                        if(tracing_block){
                            blocks.at(y).push_back(block);
                        }

                        block.width = 0;
                        tracing_block = false;

                    }
                }
                if(!tracing_block){
                    
                    block.x = x;
                    block.width = 0;
                    // is this new rect a platform?
                    if(tilemap->GetTile(x, y) >= TilemapCollisionTypeDivider::platform_collider_begins_at){
                        block.collision_mode = ColliderType::PLATFORM;
                    }
                    else{
                        block.collision_mode = ColliderType::SOLID;
                    }
                    tracing_block = true;
                }
                else{ // tracing block

                    block.width++;
                }    
            }  
        }

        if(tracing_block){
            block.width = tilemap->GetWidth() - block.x;          
            blocks.at(y).push_back(block);
        }
    }
    CreateCollidersOptimized_JoinRows(blocks, tile_size);
}

void TilemapCollider::CreateCollidersOptimized_JoinRows(std::vector<std::vector<Rect>>& blocks, sf::Vector2i tile_size){
    
    // merging all rows, storing references for where each row segment is now attached to
    std::vector<std::vector<Rect*>> block_refs;
    block_refs.resize(tilemap->GetHeight());

    // storing refs for each block, to allow a merged row to reference its new parent Rect,
    /*
        e.g. 

        Row 1 merges with Row 2, 
        Row 2 now points to Row 1
    */
    for(int y = 0; y < blocks.size(); y++){

        for(int x = 0; x < blocks.at(y).size(); x++){
            
            block_refs.at(y).push_back(&blocks.at(y).at(x));
        }
    }

    // merging rows into rects _______________________________

    for(int y = 0; y < blocks.size(); y++){

        for(int x = 0; x < blocks.at(y).size(); x++){

            if(y - 1 >= 0){

                for(int _x = 0; _x < block_refs.at(y - 1).size(); _x++){
                    
                    Rect* above = block_refs.at(y - 1).at(_x);

                    // cannot merge platforms from above, treat them as one dimensional strips
                    if(above->collision_mode == ColliderType::PLATFORM){
                        break;
                    }
                    
                    if(above->x == block_refs[y][x]->x && above->width == block_refs[y][x]->width){
                        block_refs[y][x] = above;
                        above->height++;

                        break;
                    }

                }
            }

        }

    }
    CreateCollidersOptimized_FilterAndAddReferences(block_refs, tile_size);
}

void TilemapCollider::CreateCollidersOptimized_FilterAndAddReferences(std::vector<std::vector<Rect*>>& block_refs, sf::Vector2i tile_size){
    
    // preventing two of the same rect references from being added
    std::vector<Rect*> added_rects;

    for(int y = 0; y < block_refs.size(); y++){
        for(int x = 0; x < block_refs.at(y).size(); x++){
                
            if(std::find(added_rects.begin(), added_rects.end(), block_refs[y][x]) == added_rects.end()){
                AddRect(
                    block_refs[y][x]->x * tile_size.x, 
                    block_refs[y][x]->y * tile_size.y, 
                    block_refs[y][x]->width * tile_size.x, 
                    block_refs[y][x]->height * tile_size.y,
                    block_refs[y][x]->collision_mode);
                
                added_rects.push_back(block_refs[y][x]);
            }
            
        }
    }
}