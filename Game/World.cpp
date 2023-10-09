#include "World.h"
#include <cmath>

void World::Start() {

    seed = rand() % 999999;
    perlin.reseed(seed);

    // creating each tilemap...

    half_tilemap_width = floor(tilemap_width * 0.5f);
    half_tilemap_height = floor(tilemap_height * 0.5f);
    one_divide_tilemap_width = 1 / (float)tilemap_width;
    one_divide_tilemap_height = 1 / (float)tilemap_height;

    // creating chunk vector...
    chunks.resize(width);
    for(int x = 0; x < width; x++){
        chunks[x].resize(height, nullptr);
    }

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            chunks[x][y] = GetScene()->AddObject<Object>();

            Tilemap* tmap = chunks[x][y]->AddComponent<Tilemap>();
            tmap->Load("demoTexture", 8, 8, tilemap_width, tilemap_height);
            chunks[x][y]->GetTransform()->position = sf::Vector2f(x * tilesize_x * tilemap_width, y * tilesize_y * tilemap_height);
            
            SculptingPass(x, y, tmap);

            chunks[x][y]->AddComponent<TilemapCollider>();

        }
    }
    TunnelingPass();
}

void World::SculptingPass(int x, int y, Tilemap* tilemap){

    // air
    if(y <= settings.LEVEL_AIR){
        tilemap->SetAll(-1);
    }
    // surface
    else if(y <= settings.LEVEL_DIRT){

        for(int tile_x = 0; tile_x < tilemap_width; tile_x++){
            
            int _x = tile_x + (x * tilemap_width);

            float noise_val = perlin.octave1D_01((_x * 0.04), 1, 2);
            float noise_val2 = perlin.octave1D_01((_x * 0.015), 1, 3);
            float noise_val3 = perlin.octave1D_01((_x * 0.008), 3, 1);
                                    
            float final_height = (noise_val + noise_val2 + noise_val3) * tilemap_height - ((y - settings.LEVEL_AIR) * tilemap_height);

            final_height = round(Calc::Clamp(final_height, 0, tilemap_height));

            tilemap->SetArea(10, tile_x, tile_x + 1, final_height, tilemap_height);
        }
    }
    // dirt pass ( shallow underground )
    else if(y <= settings.LEVEL_DIRT){
        tilemap->SetAll(10);
    }
    // dirt -> stone 
    else if(y <= settings.LEVEL_DIRT_TO_STONE){

        tilemap->SetAll(10);
        
        for(int tile_x = 0; tile_x < tilemap_width; tile_x++){
            
            int _x = tile_x + (x * tilemap_width);

            float noise_val = perlin.octave1D_01((_x * 0.03), 1, 2);
            float noise_val2 = perlin.octave1D_01((_x * 0.005), 1, 3);
                                    
            float final_height = (noise_val + noise_val2) * tilemap_height - ((y - settings.LEVEL_DIRT) * tilemap_height);

            final_height = round(Calc::Clamp(final_height, 0, tilemap_height));

            tilemap->SetArea(0, tile_x, tile_x + 1, final_height, tilemap_height);
        }
    }
    // stone pass (deep underground, introducing caverns )
    else{
        tilemap->SetAll(0);
        
        for(int tile_y = 0; tile_y < tilemap_height; tile_y++){
            for(int tile_x = 0; tile_x < tilemap_width; tile_x++){
                
                int _y = tile_y + (y * tilemap_height);
                int _x = tile_x + (x * tilemap_width);

                float noise_val = perlin.octave2D_01((_x * 0.04), (_y * 0.04), 2);
                noise_val += perlin.octave2D_01((_x * 0.1), (_y * 0.1), 1);
                noise_val += perlin.octave2D_01((_x * 0.013), (_y * 0.013), 1);

                if(noise_val > 1.8){
                    tilemap->SetTile(-1, tile_x, tile_y);
                }
            }
        }
    }
}

bool World::SetTile_FromWorld(int tile_index, int world_x, int world_y){

    int x = round((world_x - tilesize_x / 2.0f) / (float)tilesize_x);
    int y = round((world_y - tilesize_y / 2.0f) / (float)tilesize_y);

    sf::Vector2i chunk = ChunkFromCoord(x, y);
    if(!ChunkInBounds(chunk.x, chunk.y)){
        return false;
    }

    sf::Vector2i pos = OffsetFromCoord(x, y, chunk.x, chunk.y);
    chunks.at(chunk.x).at(chunk.y)->GetComponent<Tilemap>()->SetTile(tile_index, pos.x, pos.y);
    return true;
}

void World::TunnelingPass(){

    int spacing = 0;
    for(int x = 0; x < width * tilemap_width; x++){

        float noise_val = perlin.octave1D_01((x * 0.25), 2, 5);
        
        if(noise_val > 0.8 && spacing > settings.MIN_TUNNEL_SPACING){

            int angle = 220 + rand() % 100;
            int angle_step = -4 + rand() % 8;

            int radius_min = 3 + rand() % 5;
            int radius_max = 5 + rand() % 8;

            Tunnel(50, settings.LEVEL_AIR * tilemap_height, std::min(radius_min, radius_max), std::max(radius_min, radius_max), angle, angle_step);   
            spacing = 0;
        }

        spacing++;
        
    } 
}
void World::Tunnel(int x, int y, int radius_min, int radius_max, float angle, float angle_step){
    
    float radian_step = Calc::Radians(angle_step);
    float radians = Calc::Radians(angle);

    for(int i = 0; i < 45; i++){
        
        int rand_radius = rand() % (radius_max - radius_min + 1) + radius_min;

        SetCircle(-1, x, y, rand_radius);

        x += cos(radians) * rand_radius;
        y += sin(radians) * rand_radius;

        radians += radian_step;
    }
}

bool World::ChunkInBounds(int chunk_x, int chunk_y){
    if(chunk_x < 0 || chunk_y < 0 || chunk_x >= width || chunk_y >= height){
        return false;
    }
    return true;
}
sf::Vector2i World::ChunkFromCoord(int x, int y){
    sf::Vector2i chunk;

    chunk.x = floor((float)x * one_divide_tilemap_width);
    chunk.y = floor((float)y * one_divide_tilemap_height);

    return chunk;
}

sf::Vector2i World::OffsetFromCoord(int x, int y, int chunk_x, int chunk_y){
    
    sf::Vector2i pos;

    pos.x = x - (chunk_x * tilemap_width);
    pos.y = y - (chunk_y * tilemap_height);
    
    return pos;
}

std::vector<sf::Vector2i> World::CalculateOffsetsInRadius(int radius){

    std::vector<sf::Vector2i> in_radius;

    for(int x = -radius; x <= radius; x++) {
        
        int ydist = sqrt(radius * radius - x * x);

        for(int y = -ydist; y <= ydist; y++) {

            in_radius.push_back(sf::Vector2i(x, y));
        }
    }
    return in_radius;
}
void World::SetCircle(int tile_index, int x, int y, int radius){
    
    // we have never calculated this radius before... calculate and store
    if(radius_offsets.find(radius) == radius_offsets.end()){
        radius_offsets[radius] = CalculateOffsetsInRadius(radius);
    }

    // set all tiles
    for(auto& offset : radius_offsets[radius]){

        // converting offset to world position
        int _x = offset.x + x;
        int _y = offset.y + y;

        // what chunk is the position in
        sf::Vector2i chunk = ChunkFromCoord(_x, _y);

        // edge case for world borders
        if(!ChunkInBounds(chunk.x, chunk.y)){
            continue;
        }

        // making position relative to selected chunk
        sf::Vector2i pos = OffsetFromCoord(_x, _y, chunk.x, chunk.y);

        chunks.at(chunk.x).at(chunk.y)->GetComponent<Tilemap>()->SetTileSafe(tile_index, pos.x, pos.y);
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

                // ISSUE : CAUSING POSSIBLE MEMORY LEAK? 
                // chunks[x][y]->GetComponent<Tilemap>()->CalculateEdges();
            }
            else{
                chunks[x][y]->SetActive(false);
                chunks[x][y]->GetComponent<Tilemap>()->ClearEdges();
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