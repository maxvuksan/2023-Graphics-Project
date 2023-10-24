#pragma once
#include "BlockTypes.h"

/*
    determines if a tile is allowed to be set
*/
enum SetMode{
    OVERRIDE, // overrides everything
    ONLY_AIR, // only allows setting in air (-1 tiles)
    ONLY_BLOCK, // only allows setting on other blocks ( != -1 tiles)
};


struct WorldSettings{

    // the chunk level where generation occurs, values are required to be ascending
    int LEVEL_AIR = 1;
    int LEVEL_SURFACE = 3;
    int LEVEL_DIRT = 8;
    int LEVEL_DIRT_TO_STONE = 9;

    int MIN_TUNNEL_SPACING = 15; // the minimum distance surface tunnels can spawn be next to each other

    // percentages

    float ORE_PERCENT_MIN = 1;
    float ORE_PERCENT_MAX = 30;

    std::vector<BlockCode> SURFACE_ORE_POOL = { BlockCode::c_Stone };
    std::vector<BlockCode> CAVERN_ORE_POOL = { BlockCode::c_Dirt, BlockCode::c_Copper, BlockCode::c_Iron, BlockCode::c_Gold };
};