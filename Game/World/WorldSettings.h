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

    // the chunk level where generation occurs, values are required to be ascending
    int LEVEL_AIR = 1;
    int LEVEL_SURFACE = 3;
    int LEVEL_DIRT = 8;
    int LEVEL_DIRT_TO_STONE = 9;

    // percentages

    float SURFACE_TUNNEL_PERCENT = 1;
    float SURFACE_TUNNEL_CHANCE_DIRECTION_PERCENT = 15;
    float SURFACE_TUNNEL_SPLIT_PERCENT = 4;

    float SURFACE_GRASS_PERCENT = 40;

    float TREE_PERCENT = 0; 
    float ORE_PERCENT = 40;

    std::vector<MainBlockCode> SURFACE_ORE_POOL = { MainBlockCode::main_Stone, MainBlockCode::main_Clay, MainBlockCode::main_Copper };
    std::vector<MainBlockCode> CAVERN_ORE_POOL = { MainBlockCode::main_Dirt, MainBlockCode::main_Copper, MainBlockCode::main_Iron, MainBlockCode::main_Gold };
};