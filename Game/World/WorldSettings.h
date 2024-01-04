#pragma once
#include "../Items/ItemDictionary.h"

/*
    determines if a tile is allowed to be set
*/
enum SetMode{
    OVERRIDE, // overrides everything
    ONLY_AIR, // only allows setting in air (-1 tiles)
    ONLY_BLOCK, // only allows setting on other blocks ( != -1 tiles)
};

enum SetLocation{
    MAIN, // collidable ground
    FOREGROUND,
    BACKGROUND, 
};

struct WorldSettings{

    // scalers

    // an additional value to shift the surface lower in the world
    int NOISE_BASE = 500;

    float NOISE_1_SCALE = 0.005;
    float NOISE_1_AMPLITUDE = 50;

    // drives the amplitude of noise 2
    float NOISE_2_DRIVER_SCALE = 0.002;
    float NOISE_2_SCALE = 0.01;
    float NOISE_2_AMPLITUDE = 25;

    float NOISE_3_SCALE = 0.1;
    float NOISE_3_AMPLITUDE = 3;

    // what fraction distance from center in does outer noise begin to have an effect
    float OUTER_NOISE_THRESHHOLD = 0.1f;
    // is gradually applied at the edges of the world
    float OUTER_NOISE_SCALE = 0.009;
    float OUTER_NOISE_AMPLITUDE = 400;
    int OUTER_BASE = 1000;

    // what fraction of the world is the surface layer
    float SURFACE_THRESHOLD = 0.05;
    float DEEP_SURFACE_AMOUNT = 0.3;
    float DEEP_SURFACE_STONE_AMOUNT = 0.4;

    // caves must be this deep under surface
    float CAVE_DEPTH_MIN = 30; 
    // surface
    float CAVE_PERLIN_THRESHOLD_TOP = 0.3; 
    // bottom of world
    float CAVE_PERLIN_THRESHOLD_BOTTOM = 0.45; 

    // percentages
    float SURFACE_TUNNEL_PERCENT = 3;
    float SURFACE_TUNNEL_CHANCE_DIRECTION_PERCENT = 20;
    float SURFACE_TUNNEL_SPLIT_PERCENT = 10;

    float SURFACE_FLOOR_STICKS_PERCENT = 30;

    float SURFACE_GRASS_PERCENT = 40;

    float TREE_PERCENT = 0; // REMOVED ... ?
    float ORE_PERCENT = 40;

    std::vector<MainBlockCode> SURFACE_ORE_POOL = { MainBlockCode::main_Stone, MainBlockCode::main_CopperOre, MainBlockCode::main_Clay};
    std::vector<MainBlockCode> CAVERN_ORE_POOL = { MainBlockCode::main_Dirt, MainBlockCode::main_CopperOre, MainBlockCode::main_IronOre, MainBlockCode::main_GoldOre };
};