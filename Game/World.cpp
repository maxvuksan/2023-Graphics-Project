#include "World.h"

void World::Start(){

    // creating each tilemap...

    PerlinNoise::seed_type seed; 
    PerlinNoise perlin{ seed };

    chunks.resize(width);
    for(int x = 0; x < width; x++){
        chunks[x].resize(height, nullptr);



        for(int y = 0; y < height; y++){

            chunks[x][y] = GetScene()->AddObject<Object>();

            Tilemap* tmap = chunks[x][y]->AddComponent<Tilemap>();
            tmap->Load("demoTexture", 8, 8, tilemap_width, tilemap_height);
            chunks[x][y]->GetTransform()->position = sf::Vector2f(x * 8 * tilemap_width, y * 8 * tilemap_height);
            
            if(y < 1){

                for(int tile_x = 0; tile_x < tilemap_width; tile_x++){
                    
                    tmap->SetArea(10, tile_x, tile_x + 1, 8, tilemap_height);
                }
            }
            else if(y < 2){
                tmap->SetAll(10);
            }
            else{
                tmap->SetAll(0);
            }

        }
    }

}

void World::Update(){
    
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