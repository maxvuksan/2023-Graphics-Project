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

    main_CopperOre,
    main_IronOre,
    main_GoldOre,

    main_AncientStonePlate,
    main_AncientStonePlateCracked,
    main_AncientStonePlateShaped,
    main_AncientStonePlateChiseled,
    main_AncientStoneBricks,

    main_WoodPlanks,

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

    foreground_FloorRoot,
    foreground_Stump,

    foreground_VineBerry,

    foreground_NUMBER_OF_BLOCKS
};

// walls behind surface
enum BackgroundBlockCode{
    background_Dirt,
    background_Stone,
    background_WoodPlanks,
    background_StoneBricks,
    background_StonePlate,
    background_StonePlateCracked,

    background_NUMBER_OF_BLOCKS
};

// utility blocks occupying multiple tiles derived from UtilityStation
enum UtilityBlock{
    utility_CraftingStool,
    utility_WorkBench,
    utility_Furnace,
    utility_HeavyFurnace,
    utility_Chest,
    utility_Anvil,
    utility_Cookpot,

    utility_NUMBER_OF_BLOCKS
};