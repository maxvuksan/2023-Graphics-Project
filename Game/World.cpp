#include "World.h"
#include <cmath>

void World::Start(){

    // creating each tilemap...

    PerlinNoise::seed_type seed; 
    PerlinNoise perlin{ seed };

    half_tilemap_width = floor(tilemap_width * 0.5f);
    half_tilemap_height = floor(tilemap_height * 0.5f);

    chunks.resize(width);
    for(int x = 0; x < width; x++){
        chunks[x].resize(height, nullptr);

        for(int y = 0; y < height; y++){

            chunks[x][y] = GetScene()->AddObject<Object>();

            Tilemap* tmap = chunks[x][y]->AddComponent<Tilemap>();
            tmap->Load("demoTexture", 8, 8, tilemap_width, tilemap_height);
            chunks[x][y]->GetTransform()->position = sf::Vector2f(x * tilesize_x * tilemap_width, y * tilesize_y * tilemap_height);
            
            if(y < 2){

                for(int tile_x = 0; tile_x < tilemap_width; tile_x++){
                    
                    int _x = tile_x + (x * tilemap_width);

                    float noise_val = perlin.octave1D_01((_x * 0.05), 1, 4);
                    float noise_val2 = perlin.octave1D_01((_x * 0.02), 1, 4);
                                        
                    float final_height = (noise_val + noise_val2) * tilemap_height - (y * tilemap_height);

                    final_height = round(Calc::clamp(final_height, 0, tilemap_height));

                    tmap->SetArea(10, tile_x, tile_x + 1, final_height, tilemap_height);
                }
            }
            else{
                tmap->SetAll(0);
            }
            chunks[x][y]->AddComponent<TilemapCollider>();

        }
    }

}

void World::SetFocus(Transform* focus){
    this->focus = focus;
}

void World::Update(){
    
    if(focus == nullptr){
        std::cout << "ERROR : The world has no focus, use World::SetFocus()\n";
        return;
    }

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            int real_x = x * tilemap_width * tilesize_x + half_tilemap_width * tilesize_x;
            int real_y = y * tilemap_height * tilesize_y + half_tilemap_height * tilesize_y;
            
            int dis_x = abs(focus->position.x - real_x);
            int dis_y = abs(focus->position.y - real_y);

            if(dis_x < collider_threshold &&
               dis_y < collider_threshold){
                
                chunks[x][y]->GetComponent<TilemapCollider>()->Reset();
            }
            else{
                chunks[x][y]->GetComponent<TilemapCollider>()->ClearRects();
            }



            if(dis_x < loading_threshold &&
               dis_y < loading_threshold){
                
                chunks[x][y]->SetActive(true);
            }
            else{
                chunks[x][y]->SetActive(false);
            }
        }
    }
    /*
    return;
    if(delay_tracked > delay){

        chunks[t_index]->GetComponent<TilemapCollider>()->ClearRects();

        t_index++;
        t_index %= chunks.size();

        chunks[t_index]->GetComponent<TilemapCollider>()->Reset();

        delay_tracked = 0;
    }
    delay_tracked += Time::Dt();
    */
    
    
}