#pragma once
#include "Item.h"
#include <vector>


// enum identifier for each recipe
enum Recipes{

    recipe_Torch,
    recipe_WoodPlanks,

    recipe_Workbench,
    recipe_Furnace,
    recipe_Anvil,
    recipe_Chest,
    recipe_Cookpot,
    recipe_StoneCutter,

    recipe_Copper,
    recipe_Iron,
    recipe_Gold,

    // picaxe
    recipe_Copper_Picaxe,
    recipe_Iron_Picaxe,
    recipe_Gold_Picaxe,

    // hammer
    recipe_Copper_Hammer,
    recipe_Iron_Hammer,
    recipe_Gold_Hammer,

    // sword
    recipe_Copper_Sword,
    recipe_Iron_Sword,
    recipe_Gold_Sword,

    // sword
    recipe_Copper_Axe,
    recipe_Iron_Axe,
    recipe_Gold_Axe,

    //food
    recipe_BerryHandful,

    recipe_Platform,

    recipe_WoodBackground,

    recipe_StoneBricks,
    recipe_StonePlate,
    recipe_StonePlateCracked,
    recipe_Background_StoneBricks,
    recipe_Background_StonePlate,
    recipe_Background_StonePlateCracked,
    
    recipe_NUMBER_OF_RECIPES,
};


enum RecipeGroups{

    rgroup_Inventory,
    rgroup_Workbench,
    rgroup_Furnace,
    rgroup_Anvil,
    rgroup_Cookpot,
    rgroup_StoneCutter,

    rgroup_NUMBER_OF_RECIPE_GROUPS,
};


// a ItemCode and count grouping (e.g an ingredient could be 10 Wood)
struct Ingredient{
    ItemCode item_code;
    int count = 1;
};



// what the recipe produces and what it requires
struct RecipeData{
    Ingredient result;
    std::vector<Ingredient> ingredients; 
};


// a RecipeGroup represents a set of recipies a utility station could show, we could create groupings for each type crafting based utility station 
struct RecipeGroupData{
    int width = 0; // slot rows
    int height = 0; // slot count
    std::vector<Recipes> recipes;
};