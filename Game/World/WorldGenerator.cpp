#include "WorldGenerator.h"
#include "World.h"
#include "../Tree.h"
#include "../Utility/HibernationStation.h"

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
    IdentifySurface(); 

    CoverCavernLayerInStone();
    CarveCaves();
    OrePass();

    TunnelPass();
    IdentifySurface(); 

    CreateHibernationStructure();
    VegetationPass();

}

void WorldGenerator::SculptingPass(){
    
    // define surface curve

    surface_y_vector.clear();

    int center = (world->world_profile.width * world->tilemap_profile->width) / 2.0f;

    for(int x = 0; x < world->world_profile.width * world->tilemap_profile->width; x++){

        float surface_height = perlin.octave1D_01((x * settings.NOISE_1_SCALE), 2, 1) * settings.NOISE_1_AMPLITUDE 
                             + perlin.octave1D_01((x * settings.NOISE_3_SCALE), 2, 1) * settings.NOISE_3_AMPLITUDE 
                             + settings.NOISE_BASE;

        float noise_2_driver = Calc::Clamp(perlin.octave1D_01((x * settings.NOISE_2_DRIVER_SCALE), 1, 1), 0, 1);

        surface_height +=  perlin.octave1D_01((x * settings.NOISE_2_SCALE), 2, 2) * settings.NOISE_2_AMPLITUDE * noise_2_driver;

        // gradually apply outer specific noise further from the center
        float difference_from_center = abs(center - x) / (float)(world->world_profile.width * world->tilemap_profile->width);
        if(difference_from_center > settings.OUTER_NOISE_THRESHHOLD){
            surface_height += (perlin.octave1D_01((x * settings.OUTER_NOISE_SCALE), 2, 1) * settings.OUTER_NOISE_AMPLITUDE - settings.OUTER_BASE) * (difference_from_center - settings.OUTER_NOISE_THRESHHOLD);
        }
        

        surface_y_vector.push_back(round(Calc::Clamp(surface_height, 0, 999)));
    }

    world->world_spawn_coord = sf::Vector2i(center, surface_y_vector.at(center) - 2);

    for(int x = 0; x < world->world_profile.width; x++){
        for(int y = 0; y < world->world_profile.height; y++){

            Sculpt(x, y);
        }
    }
}
void WorldGenerator::Sculpt(int chunk_x, int chunk_y){

    Tilemap* main_tilemap = world->chunks[chunk_x][chunk_y]->GetTilemap(SetLocation::MAIN);
    Tilemap* background_tilemap = world->chunks[chunk_x][chunk_y]->GetTilemap(SetLocation::BACKGROUND);

    for(int x = 0; x < world->tilemap_profile->width; x++){
        
        int coord_x = chunk_x * world->tilemap_profile->height + x;

        int y_set_area_start = -3; 

        for(int y = 0; y <= world->tilemap_profile->height; y++){

            int coord_y = chunk_y * world->tilemap_profile->height + y;

            if(coord_y <= surface_y_vector.at(coord_x)){

                y_set_area_start = y;
            }
        }

        background_tilemap->SetArea(BackgroundBlockCode::background_Dirt, x, x + 1, Calc::Clamp(y_set_area_start + 3, 0, world->tilemap_profile->height), world->tilemap_profile->height);
        main_tilemap->SetArea(MainBlockCode::main_Dirt, x, x + 1, Calc::Clamp(y_set_area_start, 0, world->tilemap_profile->height), world->tilemap_profile->height);
    }

    /*
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
            //float noise_val3 = perlin.octave1D_01((_x * 0.008), 3, 1);
                                    
            float final_height_float = noise_val * world->tilemap_profile->height - ((y - settings.LEVEL_AIR) * world->tilemap_profile->height);

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
    */
}

void WorldGenerator::CoverCavernLayerInStone(){

    for(int x = 0; x < world->world_profile.width * world->tilemap_profile->width; x++){


        int cavern_start = settings.SURFACE_THRESHOLD * world->tilemap_profile->height * world->world_profile.height;
        int deep_surface_start = cavern_start - settings.DEEP_SURFACE_AMOUNT * settings.SURFACE_THRESHOLD * world->tilemap_profile->height * world->world_profile.height;

        // deep surface
        for(int y = deep_surface_start + surface_y_vector.at(x); y < cavern_start + surface_y_vector.at(x); y++){
            
            float noise_val = perlin.octave2D_01((x * 0.04), (y * 0.04), 2);

            if(noise_val < settings.DEEP_SURFACE_STONE_AMOUNT){
                world->SetTile(MainBlockCode::main_Stone, x, y, SetLocation::MAIN);
            }
        }

        // cavern
        for(int y = cavern_start + surface_y_vector.at(x); y < world->tilemap_profile->height * world->world_profile.height; y++){
            world->SetTile(MainBlockCode::main_Stone, x, y, SetLocation::MAIN);
            world->SetTile(BackgroundBlockCode::background_Stone, x, y, SetLocation::BACKGROUND);
        }
    }
}

void WorldGenerator::CarveCaves(){
    
    for(int y = 0; y < world->world_profile.height * world->tilemap_profile->height; y++){

        float cave_threshold = Calc::Lerp(settings.CAVE_PERLIN_THRESHOLD_TOP, settings.CAVE_PERLIN_THRESHOLD_BOTTOM, y / (float)(world->world_profile.height * world->tilemap_profile->height));

        for(int x = 0; x < world->world_profile.width * world->tilemap_profile->width; x++){

            // cave too shallow to carve
            if(surface_y_vector.at(x) + settings.CAVE_DEPTH_MIN > y){
                continue;
            }

            float noise_val = perlin.octave2D_01((x * 0.04), (y * 0.04), 2);

            if(noise_val < cave_threshold){
                world->SetTile(-1, x, y, SetLocation::MAIN);
            }
        }
    }
}

void WorldGenerator::TunnelPass(){

    int spacing_counter = 999;
    for(int x = 0; x < surface_y_vector.size(); x++){
        
        spacing_counter++;
        if(spacing_counter < settings.SURFACE_TUNNEL_MIN_SPACING){
            continue;
        }

        // create tunnel?
        if(rand() % 1000 < settings.SURFACE_TUNNEL_PERCENT * 10){
            int radius = 3 + rand() % 2;
            Tunnel(x, surface_y_vector[x], 0, radius, RandomTunnelDirection(false), 0); 
            spacing_counter = 0;
        }

    }
}

void WorldGenerator::Tunnel(float x, float y, int radius_min, int radius_max, float angle, int branch_count){
    
    if(branch_count > 1){
        return;
    }

    float radians = Calc::Radians(angle);

    float one_degree_of_radian = Calc::Radians(1);

    int length = 200 + rand() % 400;

    if(branch_count > 0){
        length *= 0.05;
    }


    for(int i = 0; i < length; i++){
        
        if(rand() % 100 < settings.SURFACE_TUNNEL_ANGLE_SHIFT_PERCENT){
            if(rand() % 100 > 50){
                radians += -one_degree_of_radian;
            }
            else{
                radians += one_degree_of_radian;
            }
        }

        if(rand() % 100 < settings.SURFACE_TUNNEL_CHANCE_DIRECTION_PERCENT){
            radians = Calc::Radians(RandomTunnelDirection());
        }

        if(rand() % 100 < settings.SURFACE_TUNNEL_SPLIT_PERCENT){
            Tunnel(x, y, radius_max, radius_max, RandomTunnelDirection(false), branch_count + 1);
        }

        world->SetCircle(-1, floor(x), floor(y), radius_max);

        x += cos(radians);
        y -= sin(radians);
    }
}

void WorldGenerator::OrePass(){
    
    for(int x = 0; x < world->world_profile.width; x++){
        for(int y = 0; y < world->world_profile.height; y++){


            // capping ore spawns in chunk
            int ores_in_chunk = 0;

            // spawns a random number of ores in a specific chunk
            float add_ore_to_chunk = rand() % 100;
            while(add_ore_to_chunk < settings.ORE_PERCENT && ores_in_chunk < settings.MAX_ORES_PER_CHUNK){

                // determining what block the vein should be

                std::vector<MainBlockCode>* ore_pool;

                if(y * world->tilemap_profile->height <= settings.SURFACE_THRESHOLD * world->world_profile.height * world->tilemap_profile->height + surface_y_vector.at(x * world->tilemap_profile->height)){
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

void WorldGenerator::IdentifySurface(){
    
    surface_y_vector.resize(world->tilemap_profile->width * world->world_profile.width, 0);

    for(int tile_x = 0; tile_x < world->tilemap_profile->width * world->world_profile.width; tile_x++){

        for(int surface_y = 0; surface_y < world->tilemap_profile->height * world->world_profile.height; surface_y++){

            int main_block = world->GetTile(tile_x, surface_y, SetLocation::MAIN);

            // found surface tile
            if(main_block != -1){

                surface_y_vector[tile_x] = surface_y;

                break;
            }    
        }
    }
}

void WorldGenerator::VegetationPass(){
    return;

    for(int i = 0; i < 500 * world->world_profile.width * world->world_profile.height; i++){

        world->SetTile(ForegroundBlockCode::foreground_Leaves, 
            rand() % (world->world_profile.width * world->world_profile.tilemap_profile.width),
            rand() % (world->world_profile.height * world->world_profile.tilemap_profile.height), SetLocation::FOREGROUND, SetMode::ONLY_BLOCK);
    }
    for(int i = 0; i < 500 * world->world_profile.width * world->world_profile.height; i++){
        world->SetTile(ForegroundBlockCode::foreground_Moss, 
            rand() % (world->world_profile.width * world->world_profile.tilemap_profile.width),
            rand() % (world->world_profile.height * world->world_profile.tilemap_profile.height), SetLocation::FOREGROUND, SetMode::ONLY_BLOCK);
    }

    for(int i = 0; i < 1000 * world->world_profile.width * world->world_profile.height; i++){

        int x = rand() % (world->world_profile.width * world->world_profile.tilemap_profile.width);
        int y = rand() % (world->world_profile.height * world->world_profile.tilemap_profile.height);

        if(world->GetTile(x, y + 1, SetLocation::MAIN) != -1 && world->GetTile(x, y, SetLocation::MAIN) == -1) {
            if(rand() % 100 > 50){
                world->SetTile(ForegroundBlockCode::foreground_FloorRoot, x, y, SetLocation::FOREGROUND);
            }
            else{
                world->SetTile(ForegroundBlockCode::foreground_Stump, x, y, SetLocation::FOREGROUND);
            }
        }
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

        else if(rand() % 100 < settings.SURFACE_FLOOR_STICKS_PERCENT){
            if(rand() % 100 > 50){
                world->SetTile(ForegroundBlockCode::foreground_FloorRoot, x,  surface_y_vector[x] - 1, SetLocation::FOREGROUND);
            }
            else{
                world->SetTile(ForegroundBlockCode::foreground_Stump, x,  surface_y_vector[x] - 1, SetLocation::FOREGROUND);
            }
        }
    }

    // create foliage
    for(int i = 0; i < settings.FOLIAGE_ATTEMPTS_PER_CHUNK * world->world_profile.width * world->world_profile.height; i++){

        int x = rand() % (world->world_profile.width * world->world_profile.tilemap_profile.width);
        int y = rand() % (world->world_profile.height * world->world_profile.tilemap_profile.height);

        sf::Vector2i chunk_coord = world->ChunkFromCoord(x, y);
        sf::Vector2i chunk_offset = world->OffsetFromCoord(x, y, chunk_coord.x, chunk_coord.y);

        // tile is solid, above tile is air
        if(world->GetTile(x, y, SetLocation::MAIN) != -1 && world->GetTile(x, y - 1, SetLocation::MAIN) == -1 && world->GetTile(x, y - 1, SetLocation::BACKGROUND) != -1){
            
            int foliage_index = rand() % foliage_NUMBER_OF_FOLIAGE;

            world->GetChunks()->at(chunk_coord.x).at(chunk_coord.y)->AddFoliage((Foliage)foliage_index, chunk_offset.x, chunk_offset.y);
        }
        
    }




    bool tree_last = false;
    // adding trees
    for(int x = 0; x < surface_y_vector.size(); x++){

        if(tree_last){
            tree_last = false;
            continue;
        }

        float noise_val = perlin.octave1D_01((x * settings.TREE_PERLIN_SCALE), 2) + perlin.octave1D_01((x * 0.5), 2);


        if(noise_val < settings.TREE_PERLIN_THRESHOLD * 2){

            // only grow trees on dirt
            if(world->GetTile(x, surface_y_vector.at(x)) != main_Dirt){
                continue;
            }

            SpawnTree(x, surface_y_vector.at(x) + 1);
            tree_last = true;
            
            if(rand() % 100 > 35){
                x++;
            }
            if(rand() % 100 > 15){
                x++;
            }
        }
    }
    
}

void WorldGenerator::CreateHibernationStructure(){
    world->GetSpawnCoordinate();

    for(int x = -3; x < 3; x++){
        for(int y = -6; y < 6; y++){

            sf::Vector2i coord = sf::Vector2i(world->GetSpawnCoordinate().x + x, world->GetSpawnCoordinate().y + y + 2);

            if(x == -3 && y == -6){

                sf::Vector2i chunk_coord = world->ChunkFromCoord(coord.x, coord.y);
                Chunk* chunk = world->GetChunks()->at(chunk_coord.x).at(chunk_coord.y);

                UtilityStation* hibernation_station = chunk->AddObjectToChunk<HibernationStation>();
                
                chunk->StoreUtilityStationReference(hibernation_station);
                hibernation_station->LinkChunk(chunk);
                hibernation_station->OnStart();
                
                hibernation_station->SetItemType(item_Utility_HibernationStation);


                hibernation_station->GetTransform()->position = world->CoordToWorld(coord.x, coord.y);
            }

            if(y < 0){
                // create air space
                world->SetTile(-1, coord.x, coord.y);
                world->SetTile(-1, coord.x, coord.y, SetLocation::FOREGROUND);
            }
            else{
                world->SetTile(main_Stone_Bricks, coord.x, coord.y);
                world->SetTile(-1, coord.x, coord.y, SetLocation::FOREGROUND);
            }

        }
    }
}

void WorldGenerator::SpawnTree(int x, int y){
    int height = settings.TREE_MIN_HEIGHT + rand() % (settings.TREE_MAX_HEIGHT - settings.TREE_MIN_HEIGHT);
    for(int h = 0; h < height; h++){
        world->SetTile(foreground_Log, x, y - h, SetLocation::FOREGROUND, SetMode::OVERRIDE);
    }
}

float WorldGenerator::RandomTunnelDirection(bool can_be_straight){
    
    int random_dir = rand() % settings.SURFACE_TUNNEL_ANGLES.size();
    return settings.SURFACE_TUNNEL_ANGLES[random_dir];
}

float WorldGenerator::EqualChancePick(float x, float y){
    if(rand() % 100 > 50){
        return x;
    }
    return y;
}