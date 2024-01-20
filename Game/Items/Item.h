#pragma once
#include <iostream>

enum ItemCode{

    item_NO_DROP,

    // main blocks
    item_Main_Dirt,
    item_Main_Stone,

    item_Main_StoneBricks,
    item_Main_StonePlate,
    item_Main_StonePlateCracked,
    
    item_Main_Clay,
    item_Main_SandStonePlate,

    item_Main_AncientStonePlate,
    item_Main_AncientStonePlateCracked,
    item_Main_AncientStonePlateShaped,
    item_Main_AncientStonePlateChiseled,
    item_Main_AncientStoneBricks,

    item_Main_WoodPlanks,

    item_Main_CopperOre,
    item_Main_IronOre,
    item_Main_GoldOre,

    item_Main_Platform,

    item_Copper,
    item_Iron,
    item_Gold,

    // background
    item_Background_Dirt,
    item_Background_Stone,
    item_Background_WoodPlanks,
    item_Background_StoneBricks,
    item_Background_StonePlate,
    item_Background_StonePlateCracked,

    // utility
    item_Utility_HibernationStation,
    item_Utility_CraftingStool,
    item_Utility_WorkBench,
    item_Utility_Furnace,
    item_Utility_StoneCutter,
    item_Utility_Chest,
    item_Utility_Anvil,
    item_Utility_Cookpot,

    // picaxe
    item_Copper_Picaxe,
    item_Iron_Picaxe,
    item_Gold_Picaxe,

    // hammer
    item_Copper_Hammer,
    item_Iron_Hammer,
    item_Gold_Hammer,

    // sword
    item_Copper_Sword,
    item_Iron_Sword,
    item_Gold_Sword,

    item_Copper_Axe,
    item_Iron_Axe,
    item_Gold_Axe,

    // resource
    item_Fibre,
    item_BigLeaf,
    item_Book,

    item_Torch,
    
    item_Sticks,

    item_Mushroom,
    item_Berries,
    item_BerryHandful,
    
    item_NUMBER_OF_ITEMS,
};


// how should the item by interpreted?
enum ItemType{

    // blocks

    type_Main,
    type_Foreground,
    type_Background,
    type_Utility,
    type_Picaxe,
    type_Hammer,
    type_Axe,
    type_Food,
    type_Resource,

    type_NUMBER_OF_TYPES
};

enum Picaxe{
    picaxe_Copper,
    picaxe_Iron,
    picaxe_Gold,

    picaxe_NUMBER_OF_PICAXES,
};

enum Hammer{
    hammer_Copper,
    hammer_Iron,
    hammer_Gold,

    hammer_NUMBER_OF_HAMMERS,
};
enum Axe{
    axe_Copper,
    axe_Iron,
    axe_Gold,

    axe_NUMBER_OF_AXES,
};

struct PicaxeData{

    int level; // how types of blocks can be mined? 
    float speed; // how fast can this picaxe mine?
};

enum Food {
    food_Mushroom,
    food_Berries,
    food_BerryHandful,

    food_NUMBER_OF_FOODS,
};

struct FoodData{
    int health_to_add;
    int hunger_to_add;
};

