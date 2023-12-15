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
    
    item_Copper,
    item_Iron,
    item_Gold,

    // background
    item_Background_Dirt,
    item_Background_Stone,

    // utility
    item_Utility_CraftingStool,
    item_Utility_WorkBench,
    item_Utility_Furnace,
    item_Utility_HeavyFurnace,
    item_Utility_Chest,

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

    // resource
    item_Fibre,
    item_BigLeaf,
    item_Book,

    item_Torch,

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
    type_Resource,

    //type_Consumable,
    //type_RangedWeapon,
    //type_MeleeWeapon,

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

struct PicaxeData{

    int level; // how types of blocks can be mined? 
    float speed; // how fast can this picaxe mine?
};


