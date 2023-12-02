#pragma once
#include <iostream>

// how should the item by interpreted?
enum ItemType{

    // blocks

    type_Main,
    type_Foreground,
    type_Background,
    type_Utility,
    type_Picaxe,

    //type_Consumable,
    //type_RangedWeapon,
    //type_MeleeWeapon,
};

enum Picaxe{
    picaxe_Copper,
    picaxe_Iron,
    picaxe_Gold,

    picaxe_NUMBER_OF_PICAXES,
};

struct PicaxeData{

    int level; // how types of blocks can be mined? 
    float speed; // how fast can this picaxe mine?
};


