#pragma once
#include "Item.h"
#include <vector>


// enum identifier for each recipe
enum Recipes{

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

    recipe_NUMBER_OF_RECIPES,
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
    int width; // slot rows
    int height; // slot count
    // note : width and height must match the provided vector dimensions
    std::vector<std::vector<Recipes>> recipes;
};

enum RecipeGroups{

    rgroup_Workbench,

    rgroup_NUMBER_OF_RECIPE_GROUPS,
};
