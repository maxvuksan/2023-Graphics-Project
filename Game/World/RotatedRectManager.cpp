#include "RotatedRectManager.h"
#include "World.h"

std::vector<RotatingRect*> RotatedRectManager::rotated_rects;
World* RotatedRectManager::world;

void RotatedRectManager::LinkWorld(World* _world){
    world = _world;
    rotated_rects.clear();
}

void RotatedRectManager::AddRect(RotatingRect* rect){
    rotated_rects.push_back(rect);    
}

void RotatedRectManager::RemoveRect(RotatingRect* rect){
    for(int i = 0; i < rotated_rects.size(); i++){
        if(rotated_rects[i] == rect){
            rotated_rects.erase(rotated_rects.begin() + i);
        }    
    }
}

void RotatedRectManager::Update(){


    for(int i = 0; i < rotated_rects.size(); i++){
        
        // ignore disabled
        if(!rotated_rects[i]->IsActive()){
            continue;
        }

        std::vector<sf::Vector2i> chunks_to_check;
    
        sf::Vector2i coords[4] = {world->WorldToCoord(rotated_rects[i]->GetTopLeft().x, rotated_rects[i]->GetTopLeft().y),
                                  world->WorldToCoord(rotated_rects[i]->GetTopRight().x, rotated_rects[i]->GetTopRight().y),
                                  world->WorldToCoord(rotated_rects[i]->GetBottomLeft().x, rotated_rects[i]->GetBottomLeft().y),
                                  world->WorldToCoord(rotated_rects[i]->GetBottomRight().x, rotated_rects[i]->GetBottomRight().y), };

        chunks_to_check.push_back(world->ChunkFromCoord(coords[0].x, coords[0].y));
    
        for(int i = 1; i < 4; i++){
            sf::Vector2i chunk_coord = world->ChunkFromCoord(coords[i].x, coords[i].y);
            if(std::find(chunks_to_check.begin(), chunks_to_check.end(), chunk_coord) != chunks_to_check.end()){
                chunks_to_check.push_back(chunk_coord);
            }
        }

        for(int c = 0; c < chunks_to_check.size(); c++){

            if(!world->ChunkInBounds(chunks_to_check[c].x, chunks_to_check[c].y)){
                return;
            }

            Chunk* chunk = world->GetChunks()->at(chunks_to_check[c].x).at(chunks_to_check[c].y);
            const std::map<int, Foliage>& foliage_map = chunk->GetFoliageMap();

            sf::Vector2f top_left;
            sf::Vector2f top_right;
            sf::Vector2f bottom_left;
            sf::Vector2f bottom_right;

            bool re_iterate = true;

            while(re_iterate){
                re_iterate = false;

                for(auto item : foliage_map){
                    
                    int x_coord = item.first % world->tilemap_profile->width;
                    int y_coord = floor((item.first - x_coord) / (float)world->tilemap_profile->width);
                    int x = x_coord * (float)world->tilemap_profile->tile_width - ItemDictionary::FOLIAGE_DATA[item.second].origin.x + 4;
                    int y = (floor(item.first - x_coord) / world->tilemap_profile->width) * (float)world->tilemap_profile->tile_height - ItemDictionary::FOLIAGE_DATA[item.second].origin.y;

                    sf::Vector2f position = chunk->GetTransform()->position + sf::Vector2f(x, y);

                    top_left = position;
                    top_right = position + sf::Vector2f(ItemDictionary::FOLIAGE_DATA[item.second].pixel_dimensions.x, 0);
                    bottom_left = position + sf::Vector2f(0, ItemDictionary::FOLIAGE_DATA[item.second].pixel_dimensions.y);
                    bottom_right = position + sf::Vector2f(ItemDictionary::FOLIAGE_DATA[item.second].pixel_dimensions.x, ItemDictionary::FOLIAGE_DATA[item.second].pixel_dimensions.y);
                
                    if(rotated_rects[i]->Overlapping(top_left, top_right, bottom_left, bottom_right)){
                        chunk->RemoveFoliage(x_coord, y_coord);
                        re_iterate = true;
                        break;
                    }
                }
            }
        }

    }

}
