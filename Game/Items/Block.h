#pragma once
#include "Item.h"

// colliable surface
enum MainBlockCode{
    main_Dirt,
    main_Stone,
    
    main_Stone_Bricks,
    main_Stone_Plate,
    main_Stone_Plate_Cracked,

    main_Clay,
    main_Sandstone_Plate,

    main_Copper,
    main_Iron,
    main_Gold,

    main_NUMBER_OF_BLOCKS
};

// overlayed ontop of surface
enum ForegroundBlockCode{
    foreground_Leaves,
    foreground_Moss,
    foreground_Grass,

    foreground_Mushroom,
    foreground_Pebbles,

    foreground_Torch,
    foreground_TorchLeft,
    foreground_TorchRight,

    foreground_Roots,
    foreground_RootsEnd,
    
    foreground_Vine,
    foreground_VineEnd,
    

    foreground_NUMBER_OF_BLOCKS
};

// walls behind surface
enum BackgroundBlockCode{
    background_Dirt,
    background_Stone,

    background_NUMBER_OF_BLOCKS
};

// utility blocks occupying multiple tiles derived from UtilityStation
enum UtilityBlock{
    utility_CraftingStool,
    utility_WorkBench,
    utility_Furnace,
    utility_HeavyFurnace,
    utility_Chest,

    utility_NUMBER_OF_BLOCKS
};