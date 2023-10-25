#include "WorldGenerator.h"
#include "World.h"

// static declarations...
PerlinNoise::seed_type WorldGenerator::seed; 
PerlinNoise WorldGenerator::perlin;
World* WorldGenerator::world = nullptr;
WorldSettings WorldGenerator::settings;

void WorldGenerator::Bind(World* _world){
    world = _world;
}

void WorldGenerator::Generate(){
    SculptingPass();
    OrePass();
}

void WorldGenerator::SculptingPass(){
    
    for(int x = 0; x < world->width; x++){
        for(int y = 0; y < world->height; y++){

            Sculpt(x, y, world->chunks[x][y]->GetTilemap(SetLocation::FOREGROUND));
        }
    }

    for(int x = 0; x < world->width; x++){
        for(int y = 3; y < world->height; y++){

            for(int t_x = 0; t_x < world->tilemap_width; t_x++){
                for(int t_y = 0; t_y < world->tilemap_width; t_y++){
                    world->chunks[x][y]->SetTile(1, t_x, t_y, SetLocation::BACKGROUND);
                }
            }
        }
    }


}
void WorldGenerator::Sculpt(int x, int y, Tilemap* tilemap){

    // air
    if(y <= settings.LEVEL_AIR){
        tilemap->SetAll(-1);
    }
    // surface
    else if(y <= settings.LEVEL_DIRT){

        for(int tile_x = 0; tile_x < world->tilemap_width; tile_x++){
            
            int _x = tile_x + (x * world->tilemap_width);

            float noise_val = perlin.octave1D_01((_x * 0.04), 1, 2);
            float noise_val2 = perlin.octave1D_01((_x * 0.015), 1, 3);
            float noise_val3 = perlin.octave1D_01((_x * 0.008), 3, 1);
                                    
            float final_height = (noise_val + noise_val2 + noise_val3) * world->tilemap_height - ((y - settings.LEVEL_AIR) * world->tilemap_height);

            final_height = round(Calc::Clamp(final_height, 0, world->tilemap_height));

            tilemap->SetArea(BlockCode::c_Dirt, tile_x, tile_x + 1, final_height, world->tilemap_height);
        }
    }
    // dirt pass ( shallow underground )
    else if(y <= settings.LEVEL_DIRT){
        tilemap->SetAll(BlockCode::c_Dirt);
    }
    // dirt -> stone 
    else if(y <= settings.LEVEL_DIRT_TO_STONE){

        tilemap->SetAll(BlockCode::c_Dirt);
        
        for(int tile_x = 0; tile_x < world->tilemap_width; tile_x++){
            
            int _x = tile_x + (x * world->tilemap_width);

            float noise_val = perlin.octave1D_01((_x * 0.03), 1, 2);
            float noise_val2 = perlin.octave1D_01((_x * 0.005), 1, 3);
                                    
            float final_height = (noise_val + noise_val2) * world->tilemap_height - ((y - settings.LEVEL_DIRT) * world->tilemap_height);

            final_height = round(Calc::Clamp(final_height, 0, world->tilemap_height));

            tilemap->SetArea(BlockCode::c_Stone, tile_x, tile_x + 1, final_height, world->tilemap_height);
        }
    }
    // stone pass (deep underground, introducing caverns )
    else{
        tilemap->SetAll(BlockCode::c_Stone);
        
        for(int tile_y = 0; tile_y < world->tilemap_height; tile_y++){
            for(int tile_x = 0; tile_x < world->tilemap_width; tile_x++){
                
                int _y = tile_y + (y * world->tilemap_height);
                int _x = tile_x + (x * world->tilemap_width);

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


void WorldGenerator::TunnelingPass(){

    int spacing = 0;
    for(int x = 0; x < world->width * world->tilemap_width; x++){

        float noise_val = perlin.octave1D_01((x * 0.02), 2, 5);
        
        if(noise_val > 0.7 && spacing > settings.MIN_TUNNEL_SPACING){

            int angle = 220 + rand() % 100;
            int angle_step = -4 + rand() % 8;

            int radius_min = 3 + rand() % 5;
            int radius_max = 5 + rand() % 8;

            Tunnel(30 + rand() % 60, settings.LEVEL_AIR * world->tilemap_height, std::min(radius_min, radius_max), std::max(radius_min, radius_max), angle, angle_step);   
            spacing = 0;
        }

        spacing++;
        
    } 
}
void WorldGenerator::Tunnel(int x, int y, int radius_min, int radius_max, float angle, float angle_step){
    
    float radian_step = Calc::Radians(angle_step);
    float radians = Calc::Radians(angle);

    int length = 40 + rand() % 60;

    for(int i = 0; i < length; i++){
        
        int rand_radius = rand() % (radius_max - radius_min + 1) + radius_min;

        world->SetCircle(-1, x, y, rand_radius);

        x += cos(radians) * rand_radius;
        y += sin(radians) * rand_radius;

        radians += radian_step;
    }
}

void WorldGenerator::OrePass(){
    
    for(int x = 0; x < world->width; x++){
        for(int y = 0; y < world->height; y++){


            // gradually increasing chance for veins to spawn
            float ore_percent = Calc::Lerp(settings.ORE_PERCENT_MIN, settings.ORE_PERCENT_MAX, y / (float)world->height);

            // capping ore spawns in chunk
            int ores_in_chunk = 0;

            // spawns a random number of ores in a specific chunk
            float add_ore_to_chunk = rand() % 100;
            while(add_ore_to_chunk < ore_percent && ores_in_chunk < 4){

                // determining what block the vein should be

                std::vector<BlockCode>* ore_pool;

                if(y <= settings.LEVEL_DIRT_TO_STONE){
                    ore_pool = &settings.SURFACE_ORE_POOL;
                }
                else{
                    ore_pool = &settings.CAVERN_ORE_POOL;
                }

                int tile_index = ore_pool->at(rand() % ore_pool->size()); 


                ores_in_chunk++;

                // giving the vein a offset (to make it look more natural, less chunk based)
                int x_shift = rand() % (world->tilemap_width * 2) - world->tilemap_width;
                int y_shift = rand() % (world->tilemap_height * 2) - world->tilemap_height;
           
                SpreadOre(tile_index, x * world->tilemap_width + x_shift, y * world->tilemap_height + y_shift, 1, 3, 70, SetMode::ONLY_BLOCK);
            }
        }
    }
}

void WorldGenerator::SpreadOre(int tile_index, int x, int y, int radius_min, int radius_max, int spread_chance, SetMode set_mode, int spread_count){

    // prevent ores from spreading too much
    if(spread_count > 7){
        return;
    }

    float spread = rand() % 100;
    if(spread < spread_chance){

        int x_shift = rand() % (radius_max * 2) - radius_max;
        int y_shift = rand() % (radius_max * 2) - radius_max;
                

        SpreadOre(tile_index, x + x_shift, y + y_shift, radius_min, radius_max, spread_chance, set_mode, spread_count++);
    }
    
    int rand_radius = rand() % (radius_max - radius_min + 1) + radius_min;

    world->SetCircle(tile_index, x, y, rand_radius, SetLocation::FOREGROUND, set_mode);
}
