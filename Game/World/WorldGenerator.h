#pragma once
#include "../../Amber/Framework.h"
#include "WorldSettings.h"
#include "Chunk.h"

class World;

/*
    Generating the terrain for a specific world
*/
class WorldGenerator{

    friend class CommandParser;

    public:
        // bind the world we want to target
        static void Bind(World* world);

        // begin generation
        static void Generate();

        // shaping the terrain (hills, caves etc...)
        static void SculptingPass();
        static void Sculpt(int chunk_x, int chunk_y);

        // creates the stone layer for the cavern
        static void CoverCavernLayerInStone();
        static void CarveCaves();

        // creating caves with direction
        static void Tunnel(int x, int y, int radius_min, int radius_max, float angle, int branch_count);

        static float RandomTunnelDirection(bool can_be_straight = true);
        // @ returns a random choice between the two given floats
        static float EqualChancePick(float x, float y);

        // iterates over the scultped terrain determining where the top most tiles are
        static void IdentifySurface();
        static void TunnelPass();
        static void VegetationPass();
        // placing ores 
        static void OrePass();
        /*  
            recursivley creates ore veins like shapes
            @param spread_chance a value between 0 -> 100, the chance which a function will call itself
        */
        static void SpreadOre(int tile_index, int x, int y, int radius_min, int radius_max, int spread_chance, SetMode set_mode = SetMode::OVERRIDE, int spread_count = 0);

    private:

        // holds the surface values of the world
        static std::vector<int> surface_y_vector;

        static PerlinNoise::seed_type seed; 
        static PerlinNoise perlin;

        static World* world;
        static WorldSettings settings;
};