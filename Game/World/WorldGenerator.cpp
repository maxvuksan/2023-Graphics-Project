#include "WorldGenerator.h"
#include "World.h"
#include "../Tree.h"

// static declarations...
PerlinNoise::seed_type WorldGenerator::seed; 
PerlinNoise WorldGenerator::perlin;
World* WorldGenerator::world = nullptr;
WorldSettings WorldGenerator::settings;

std::vector<int> WorldGenerator::surface_y_vector;

void WorldGenerator::Bind(World* _world){
    world = _world;
}

void WorldGenerator::Generate(){

    srand(time(NULL));
    seed = time(NULL);
    perlin = PerlinNoise{ seed };

    SculptingPass();
    OrePass();

    EstablishSurface(); 
    TunnelPass();
    EstablishSurface(); 
    VegetationPass();
}

void WorldGenerator::SculptingPass(){
    
    for(int x = 0; x < world->world_profile.width; x++){
        
        for(int y = 0; y < world->world_profile.height; y++){

            Sculpt(x, y);
        }
    }
}
void WorldGenerator::Sculpt(int x, int y){

    Tilemap* main_tilemap = world->chunks[x][y]->GetTilemap(SetLocation::MAIN);
    Tilemap* background_tilemap = world->chunks[x][y]->GetTilemap(SetLocation::BACKGROUND);

    // air
    if(y <= settings.LEVEL_AIR){
        main_tilemap->SetAll(-1);
    }
    // surface
    else if(y <= settings.LEVEL_DIRT){

        for(int tile_x = 0; tile_x < world->tilemap_profile->width; tile_x++){
            
            int _x = tile_x + (x * world->tilemap_profile->width);

            float noise_val = perlin.octave1D_01((_x * 0.04), 1, 2);
            float noise_val2 = perlin.octave1D_01((_x * 0.015), 1, 3);
            float noise_val3 = perlin.octave1D_01((_x * 0.008), 3, 1);
                                    
            float final_height_float = (noise_val + noise_val2 + noise_val3) * world->tilemap_profile->height - ((y - settings.LEVEL_AIR) * world->tilemap_profile->height);

            int final_height = round(Calc::Clamp(final_height_float, 0, world->tilemap_profile->height));
            int final_height_background = Calc::Clamp(final_height_float + 3, 0, world->tilemap_profile->height); // makes backgrounds 3 tiles below surface

            background_tilemap->SetArea(MainBlockCode::main_Dirt, tile_x, tile_x + 1, final_height_background, world->tilemap_profile->height);
            main_tilemap->SetArea(MainBlockCode::main_Dirt, tile_x, tile_x + 1, final_height, world->tilemap_profile->height);
        }
    }
    // dirt pass ( shallow underground )
    else if(y <= settings.LEVEL_DIRT){
        main_tilemap->SetAll(MainBlockCode::main_Dirt);
        main_tilemap->SetAll(MainBlockCode::main_Stone);
    }
    // dirt -> stone 
    else if(y <= settings.LEVEL_DIRT_TO_STONE){

        main_tilemap->SetAll(MainBlockCode::main_Dirt);
        background_tilemap->SetAll(MainBlockCode::main_Dirt);

        for(int tile_x = 0; tile_x < world->tilemap_profile->width; tile_x++){
            
            int _x = tile_x + (x * world->tilemap_profile->width);

            float noise_val = perlin.octave1D_01((_x * 0.03), 1, 2);
            float noise_val2 = perlin.octave1D_01((_x * 0.005), 1, 3);
                                    
            float final_height_float = (noise_val + noise_val2) * world->tilemap_profile->height - ((y - settings.LEVEL_DIRT) * world->tilemap_profile->height);

            int final_height = round(Calc::Clamp(final_height_float, 0, world->tilemap_profile->height));
            int final_height_background = Calc::Clamp(final_height_float + 3, 0, world->tilemap_profile->height); // makes backgrounds 3 tiles below surface

            background_tilemap->SetArea(MainBlockCode::main_Stone, tile_x, tile_x + 1, final_height_background, world->tilemap_profile->height);
            main_tilemap->SetArea(MainBlockCode::main_Stone, tile_x, tile_x + 1, final_height, world->tilemap_profile->height);
        }
    }
    // stone pass (deep underground, introducing caverns )
    else{
        main_tilemap->SetAll(MainBlockCode::main_Stone);
        background_tilemap->SetAll(MainBlockCode::main_Stone);

        for(int tile_y = 0; tile_y < world->tilemap_profile->height; tile_y++){
            for(int tile_x = 0; tile_x < world->tilemap_profile->width; tile_x++){
                
                int _y = tile_y + (y * world->tilemap_profile->height);
                int _x = tile_x + (x * world->tilemap_profile->width);

                float noise_val = perlin.octave2D_01((_x * 0.04), (_y * 0.04), 2);
                noise_val += perlin.octave2D_01((_x * 0.1), (_y * 0.1), 1);
                noise_val += perlin.octave2D_01((_x * 0.013), (_y * 0.013), 1);

                if(noise_val > 1.8){
                    main_tilemap->SetTile(-1, tile_x, tile_y);
                }
            }
        }
    }
}

void WorldGenerator::Tunnel(int x, int y, int radius_min, int radius_max, float angle, int branch_count){
    
    if(branch_count > 1){
        return;
    }

    float radians = Calc::Radians(angle);

    int length = 30 + rand() % 50;

    for(int i = 0; i < length; i++){
        
        int rand_radius = rand() % (radius_max - radius_min + 1) + radius_min;

        angle += -1 + rand() % 2;
        if(rand() % 100 < settings.SURFACE_TUNNEL_CHANCE_DIRECTION_PERCENT){
            angle = RandomTunnelDirection();
        }

        if(rand() % 100 < settings.SURFACE_TUNNEL_SPLIT_PERCENT){
            Tunnel(x, y, radius_min - 1, radius_max - 1, RandomTunnelDirection(), branch_count + 1);
        }

        world->SetCircle(-1, x, y, rand_radius);

        x += cos(radians) * rand_radius;
        y -= sin(radians) * rand_radius;
    }
}

void WorldGenerator::OrePass(){
    
    for(int x = 0; x < world->world_profile.width; x++){
        for(int y = 0; y < world->world_profile.height; y++){


            // capping ore spawns in chunk
            int ores_in_chunk = 0;

            // spawns a random number of ores in a specific chunk
            float add_ore_to_chunk = rand() % 100;
            while(add_ore_to_chunk < settings.ORE_PERCENT && ores_in_chunk < 4){

                // determining what block the vein should be

                std::vector<MainBlockCode>* ore_pool;

                if(y <= settings.LEVEL_DIRT_TO_STONE){
                    ore_pool = &settings.SURFACE_ORE_POOL;
                }
                else{
                    ore_pool = &settings.CAVERN_ORE_POOL;
                }

                int tile_index = ore_pool->at(rand() % ore_pool->size()); 


                ores_in_chunk++;

                // giving the vein a offset (to make it look more natural, less chunk based)
                int x_shift = rand() % (world->tilemap_profile->width * 2) - world->tilemap_profile->width;
                int y_shift = rand() % (world->tilemap_profile->height * 2) - world->tilemap_profile->height;
           
                SpreadOre(tile_index, x * world->tilemap_profile->width + x_shift, y * world->tilemap_profile->height + y_shift, 1, 3, 70, SetMode::ONLY_BLOCK);
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

    world->SetCircle(tile_index, x, y, rand_radius, SetLocation::MAIN, set_mode);
}

void WorldGenerator::EstablishSurface(){
    
    surface_y_vector.resize(world->tilemap_profile->width * world->world_profile.width, 0);

    for(int tile_x = 0; tile_x < world->tilemap_profile->width * world->world_profile.width; tile_x++){

        for(int surface_y = settings.LEVEL_AIR * world->tilemap_profile->height; surface_y < world->tilemap_profile->height * world->world_profile.height; surface_y++){

            int main_block = world->GetTile(tile_x, surface_y, SetLocation::MAIN);

            // found surface tile
            if(main_block != -1){

                surface_y_vector[tile_x] = surface_y;

                break;
            }    
        }
    }
}

void WorldGenerator::TunnelPass(){

    for(int x = 0; x < surface_y_vector.size(); x++){
        
        // create tunnel?
        if(rand() % 100 < settings.SURFACE_TUNNEL_PERCENT){
            Tunnel(x, surface_y_vector[x], 2, 6, RandomTunnelDirection(), 0); 
        }

    }
}

void WorldGenerator::VegetationPass(){

    for(int i = 0; i < 10000; i++){
        world->SetTile(ForegroundBlockCode::foreground_Leaves, 
            rand() % (world->world_profile.width * world->world_profile.tilemap_profile.width),
            rand() % (world->world_profile.height * world->world_profile.tilemap_profile.height), SetLocation::FOREGROUND, SetMode::ONLY_BLOCK);
    }
    for(int i = 0; i < 10000; i++){
        world->SetTile(ForegroundBlockCode::foreground_Moss, 
            rand() % (world->world_profile.width * world->world_profile.tilemap_profile.width),
            rand() % (world->world_profile.height * world->world_profile.tilemap_profile.height), SetLocation::FOREGROUND, SetMode::ONLY_BLOCK);
    }

    // add grass
    for(int x = 0; x < surface_y_vector.size(); x++){
       
        signed_byte foreground_block = world->GetTile(x, surface_y_vector[x], SetLocation::FOREGROUND); 

        // add grass if veg already present
        if(foreground_block == foreground_Moss || foreground_block == foreground_Leaves){
            world->SetTile(foreground_Grass, x, surface_y_vector[x] - 1, SetLocation::FOREGROUND);
        }
        // create additional grass
        else if(rand() % 100 < settings.SURFACE_GRASS_PERCENT){
            world->SetTile(foreground_Moss, x, surface_y_vector[x], SetLocation::FOREGROUND);
            world->SetTile(foreground_Grass, x, surface_y_vector[x] - 1, SetLocation::FOREGROUND);
        }
    }

    // adding trees
    for(int x = 0; x < surface_y_vector.size(); x++){
        if(rand() % 100 < settings.TREE_PERCENT){

            sf::Vector2i chunk_coord = world->ChunkFromCoord(x, surface_y_vector[x]);
            Chunk* chunk = world->GetChunks()->at(chunk_coord.x).at(chunk_coord.y);

            Object* tree = chunk->AddObjectToChunk<Tree>();
            tree->GetTransform()->position = sf::Vector2f(world->CoordToWorld(x, surface_y_vector[x])) + sf::Vector2f(-96,32 + 32);
        }
    }
    
}

float WorldGenerator::RandomTunnelDirection(){
    return EqualChancePick(190 + rand() % 60, 290 + rand() % 60);
}

float WorldGenerator::EqualChancePick(float x, float y){
    if(rand() % 100 > 50){
        return x;
    }
    return y;
}