#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>
#include "../../Amber/Framework.h"

#include "Block.h"
#include "Sprites.h"
#include "Recipes.h"
#include "ItemSound.h"
#include "Foliage.h"
#include "Water.h"
#include "IndexPools.h"
#include "BackgroundStructures.h"

/*

    Idea : 

    Fridge, prevents food expiration
*/

// how does the foreground reacted to its surrounding main tiles
enum class ForegroundBehaviour{

    NONE, // can sit freely in the air
    ABOVE_BLOCK, // must have a block below, no block in position
    ON_BLOCK, // position must have a block
    VINE, // have either a main tile above or another vine foreground tile
    TORCH, // can go on background walls, main floors and walls, essentially must be attached somewhere
    PLATFORM,
};

struct BlockData{

    int durability = 100;
    sf::Color base_colour = sf::Color(0,0,0); // used for minimap
    ItemCode pickup = item_NO_DROP; // what item it drops
    ForegroundBehaviour behaviour = ForegroundBehaviour::NONE;
    bool blocks_light = true; // does light pass through this block?
};

struct ItemData{
    
    std::string name;
    ItemType type;
    int code_in_type; // code within specific item type bracket
    SpriteInventory inventory_sprite = isprite_USE_MAIN_SPRITE;
    ItemSound inventory_sound = sound_Rubble;
};

enum class UtilityBreakingBehaviour{

    ALWAYS, 
    IF_EMPTY, // will be used for containers
};

struct UtilityBlockData{
    BlockData block_data;

    sf::Vector2i footprint; // how many tiles the object takes up
    sf::Vector2i texture_coordinate; // which coordinate on the texture is it, assuming the texture follows an 8px grid
    UtilityBreakingBehaviour breaking_behaviour = UtilityBreakingBehaviour::ALWAYS;
};

// foliage refers to sprites which overlay the tiled grid,
// there position still lies on the grid but may overflow there assigned cell


struct FoliageData{
    sf::Vector2i pixel_dimensions;
    sf::Vector2i pixel_position_on_sprite_sheet;
    sf::Vector2i origin; // what position in foliage sprite acts as the origin
    ItemCode pickup = item_NO_DROP;
};

namespace ItemDictionary {

    const sf::Color torch_colour(255, 215, 200);
    const float torch_propogate_decay = 0.065f;

    const int tile_size = 8;
    const int half_tile_size = 4;
    const int inventory_item_tile_size = 16;
    
    const int main_tiles_sprites_per_row = 3;
    const int background_tiles_sprites_per_row = 6;
    const int foreground_tiles_sprites_per_row = 3;
    const int inventory_sprites_per_row = 12;

    const short TYPE_STACK_SIZES[type_NUMBER_OF_TYPES] = {
        999,      //type_Main,
        999,       //type_Foreground,
        999,        //type_Background,
        99,     //type_Utility,
        1,      //type_Picaxe,
        1,      //type_Hammer,
        1,      //type_Axe
        99,     //type_Food
        99      //type_Resource  
    };

    // label of sound in AssetManager
    const std::string INVENTORY_SOUNDS[sound_NUMBER_OF_PICKUP_SOUNDS] = {
        "item_metal",
        "item_rubble",
        "item_soft"
    };


    const BackgroundStructreData BACKGROUND_STRUCTURES[structure_NUMBER_OF_STRUCTURES] = {
        { "Boiler Plate", sf::Vector2i(0,0), sf::Vector2i(56, 72) },

        { "Chamber Top", sf::Vector2i(0,80), sf::Vector2i(56, 8) },
        { "Chamber", sf::Vector2i(0,96), sf::Vector2i(56, 40) },
        { "Chamber Bottom", sf::Vector2i(0,144), sf::Vector2i(56, 8) },

        { "Support Horizontal", sf::Vector2i(64,0), sf::Vector2i(88, 56) },
        { "Support Vertical", sf::Vector2i(64,64), sf::Vector2i(56, 88) },
        { "Support Boiler plate", sf::Vector2i(160,0), sf::Vector2i(56, 56) },
    };


    /*
        provide setting multiple blocks under one item code

        e.g. placing dirt could place... providing a more textured enviroment

        main_Dirt,
        main_CourseDirt,
        main_Stone,
    */
    const std::vector<signed_byte> INDEX_POOLS[pool_NUMBER_OF_INDEX_POOLS] = {
        // pool_SurfaceLayer
        { main_Dirt, main_Dirt, main_Dirt, main_Dirt, main_Dirt, main_Dirt,
          main_CourseDirt, main_CourseDirt, main_CourseDirt, main_CourseDirt,
          main_Stone, main_Stone,
          main_CopperOre,
        },
        // pool_SurfaceLayerBackground
        {
            background_Dirt, background_Dirt, background_Dirt, background_Dirt, background_Dirt, background_Dirt, background_Dirt,
            background_DirtWithRocks,
        },

        // pool_CavernLayer
        { main_Stone, main_Stone, main_Stone, main_Stone, main_Stone, main_Stone,
          main_Stone, main_Stone, main_Stone, main_Stone, main_CourseDirt,
          main_CopperOre,
        },
        // pool_CavernLayerBackground
        {
            background_Stone, background_Stone, background_Stone, background_Stone,
            background_StoneWithCracks,
        },
    };

    const FoliageData FOLIAGE_DATA[foliage_NUMBER_OF_FOLIAGE] = {
        {sf::Vector2i(48,48), sf::Vector2i(0,0), sf::Vector2i(23,47), item_Fibre},
        {sf::Vector2i(16,16), sf::Vector2i(48,0), sf::Vector2i(8,8)},
        {sf::Vector2i(16,32), sf::Vector2i(48,16), sf::Vector2i(8,32), item_Fibre},
        {sf::Vector2i(16,16), sf::Vector2i(64,0), sf::Vector2i(8,16)},
        {sf::Vector2i(16,32), sf::Vector2i(64,16), sf::Vector2i(8,32), item_Fibre},
        {sf::Vector2i(16,16), sf::Vector2i(80,0), sf::Vector2i(8,8), item_Berries},
        {sf::Vector2i(16,16), sf::Vector2i(80,16), sf::Vector2i(8,16), item_Sticks},
        {sf::Vector2i(16,16), sf::Vector2i(80,32), sf::Vector2i(8,16), item_Sticks},
    };

    const BlockData MAIN_BLOCK_DATA[main_NUMBER_OF_BLOCKS]{

        {6, sf::Color(109,62,62), item_Main_Dirt}, 
        {8, sf::Color(87,81,107), item_Main_Stone},

        {12, sf::Color(135,135,161), item_Main_StoneBricks},
        {12, sf::Color(135,135,161), item_Main_StonePlate},
        {12, sf::Color(135,135,161), item_Main_StonePlateCracked},

        {5, sf::Color(179,135,94), item_Main_Clay},
        {12, sf::Color(179,135,94), item_Main_SandStonePlate},

        {12, sf::Color(171,110,83), item_Main_CopperOre},
        {14, sf::Color(199,144,120), item_Main_IronOre},
        {16, sf::Color(197,151,90), item_Main_GoldOre},

        {12, sf::Color(79,67,100), item_Main_AncientStonePlate},
        {12, sf::Color(79,67,100), item_Main_AncientStonePlateCracked},
        {12, sf::Color(79,67,100), item_Main_AncientStonePlateShaped},
        {12, sf::Color(79,67,100), item_Main_AncientStonePlateChiseled},
        {12, sf::Color(79,67,100), item_Main_AncientStoneBricks},


        {8, sf::Color(186, 97, 81), item_Main_WoodPlanks},
        {6, sf::Color(80,62,62), item_Main_Dirt}, 
        {8, sf::Color(47,41,57), item_Main_Stone},

        // wooden platforms
        {2, sf::Color(186, 97, 81), item_Main_Platform, ForegroundBehaviour::PLATFORM, false},
        {2, sf::Color(186, 97, 81), item_Main_Platform, ForegroundBehaviour::PLATFORM, false},     
        {2, sf::Color(186, 97, 81), item_Main_Platform, ForegroundBehaviour::PLATFORM, false},

        {2, sf::Color(186, 97, 81), item_Wood_Ladder, ForegroundBehaviour::NONE, false},
    };

    const BlockData FOREGROUND_BLOCK_DATA[foreground_NUMBER_OF_BLOCKS]{

        {0, sf::Color::Transparent, item_Fibre, ForegroundBehaviour::ON_BLOCK},
        {0, sf::Color(73, 109, 8), item_NO_DROP, ForegroundBehaviour::ON_BLOCK},
        {0, sf::Color(73, 109, 8), item_NO_DROP, ForegroundBehaviour::ABOVE_BLOCK},

        // mushroom, floor stone
        {0, sf::Color(145, 71, 63), item_Mushroom, ForegroundBehaviour::ABOVE_BLOCK},
        {0, sf::Color(145, 71, 63), item_Main_Stone, ForegroundBehaviour::ABOVE_BLOCK},

        // torch
        {0, sf::Color(252, 197, 195), item_Torch, ForegroundBehaviour::TORCH},
        {0, sf::Color(252, 197, 195), item_Torch, ForegroundBehaviour::TORCH},
        {0, sf::Color(252, 197, 195), item_Torch, ForegroundBehaviour::TORCH},

        // roots
        {0, sf::Color(145, 71, 63), item_Sticks, ForegroundBehaviour::VINE},
        {0, sf::Color(145, 71, 63), item_Sticks, ForegroundBehaviour::VINE},

        // vines
        {0, sf::Color(73, 109, 8), item_NO_DROP, ForegroundBehaviour::VINE},
        {0, sf::Color(73, 109, 8), item_NO_DROP, ForegroundBehaviour::VINE},

        // floor root, stump
        {0, sf::Color(73, 109, 8), item_Sticks, ForegroundBehaviour::ABOVE_BLOCK},
        {0, sf::Color(73, 109, 8), item_Sticks, ForegroundBehaviour::ABOVE_BLOCK},

        // vine berry
        {0, sf::Color(73, 109, 8), item_Berries, ForegroundBehaviour::VINE},

        // log
        {20, sf::Color(186, 97, 81), item_Main_WoodPlanks},
        // concrete
        {5, sf::Color(186, 97, 81)}, 
        {5, sf::Color(186, 97, 81)}, 
    };

    const BlockData BACKGROUND_BLOCK_DATA[background_NUMBER_OF_BLOCKS]{
        
        {6, sf::Color(89,42,42), item_Background_Dirt},
        {8, sf::Color(67,61,87), item_Background_Stone},
        {6, sf::Color(98,43,52), item_Background_WoodPlanks},
        {8, sf::Color(66,61,82), item_Background_StoneBricks},
        {6, sf::Color(66,61,82), item_Background_StonePlate},
        {8, sf::Color(66,61,82), item_Background_StonePlateCracked},

        {6, sf::Color(89,42,42), item_Background_Dirt},
        {8, sf::Color(67,61,87), item_Background_Stone},
    };

    const UtilityBlockData UTILITY_BLOCK_DATA[utility_NUMBER_OF_BLOCKS]{
        {{999999999, sf::Color(50,50,50), item_NO_DROP}, sf::Vector2i(6,6), sf::Vector2i(0,4)},

        {{5, sf::Color(50,50,50), item_Utility_CraftingStool}, sf::Vector2i(1,1), sf::Vector2i(0,0)},
        {{5, sf::Color(50,50,50), item_Utility_WorkBench}, sf::Vector2i(2,2), sf::Vector2i(1,0)},
        {{5, sf::Color(50,50,50), item_Utility_Furnace}, sf::Vector2i(2,2), sf::Vector2i(3,0)},
        {{5, sf::Color(50,50,50), item_Utility_StoneCutter}, sf::Vector2i(2,2), sf::Vector2i(5,0)},
        {{5, sf::Color(50,50,50), item_Utility_Chest}, sf::Vector2i(2, 2), sf::Vector2i(0,2), UtilityBreakingBehaviour::IF_EMPTY},
        {{5, sf::Color(50,50,50), item_Utility_Anvil}, sf::Vector2i(2, 2), sf::Vector2i(2,2)},
        {{5, sf::Color(50,50,50), item_Utility_Cookpot}, sf::Vector2i(2,2), sf::Vector2i(4,2)}
    };

    const PicaxeData PICAXE_DATA[picaxe_NUMBER_OF_PICAXES]{
        {1, 2.0f}, // Copper
        {1, 2.6f}, // Iron
        {2, 100.1f} // Gold
    };

    const PicaxeData HAMMER_DATA[hammer_NUMBER_OF_HAMMERS]{
        {1, 2.0f}, // Copper
        {1, 2.6f}, // Iron
        {2, 13.1f} // Gold
    };
    const PicaxeData AXE_DATA[axe_NUMBER_OF_AXES]{
        {1, 2.0f}, // Copper
        {1, 2.6f}, // Iron
        {2, 13.1f} // Gold
    };

    const FoodData FOOD_DATA[food_NUMBER_OF_FOODS]{
        {1, 10}, // mushroom
        {5, 10}, // berry
        {40, 45} // berry handful
    };

    const RecipeData RECIPE_DATA[recipe_NUMBER_OF_RECIPES]{

        // torch
        { { item_Torch, 3}, {{ item_Fibre, 1}} },
        { { item_Main_WoodPlanks}, {{ item_Sticks, 4}}},
        // workbench
        { {item_Utility_WorkBench}, {{ item_Main_WoodPlanks, 15}}},
        // furnace
        { {item_Utility_Furnace}, {{ item_Main_Stone, 25}}},
        // anvil
        { {item_Utility_Anvil}, {{ item_Iron, 15}}},
        // chest
        { {item_Utility_Chest}, {{ item_Sticks, 30}, {item_Copper, 4}}},
        // cookpot
        { {item_Utility_Cookpot}, {{ item_Sticks, 15}, {item_Iron, 4}, {item_Main_Stone, 10}}},
        { {item_Utility_StoneCutter}, {{item_Main_Stone, 15}, {item_Iron, 5}}},

        // ores
        { {item_Copper}, {{ item_Main_CopperOre, 1}}},
        { {item_Iron}, {{ item_Main_IronOre, 1}}},
        { {item_Gold}, {{ item_Main_GoldOre, 1}}},

        // picaxe
        { {item_Copper_Picaxe}, {{item_Copper, 6}, {item_Sticks}}},
        { {item_Iron_Picaxe}, {{item_Iron, 6}, {item_Sticks}}},
        { {item_Gold_Picaxe}, {{item_Gold, 6}, {item_Sticks}}},

        // hammer
        { {item_Copper_Hammer}, {{item_Copper, 6}, {item_Sticks}}  },
        { {item_Iron_Hammer}, {{item_Iron, 6}, {item_Sticks}}  },
        { {item_Gold_Hammer}, {{item_Gold, 6}, {item_Sticks}}  },

        // sword
        { {item_Copper_Sword}, {{item_Copper, 6}, {item_Sticks}}  },
        { {item_Iron_Sword}, {{item_Iron, 6}, {item_Sticks}}  },
        { {item_Gold_Sword}, {{item_Gold, 6}, {item_Sticks}}  },

        // axe
        { {item_Copper_Axe}, {{item_Copper, 6}, {item_Sticks}}  },
        { {item_Iron_Axe}, {{item_Iron, 6}, {item_Sticks}}  },
        { {item_Gold_Axe}, {{item_Gold, 6}, {item_Sticks}}  },

        // food
        { {item_BerryHandful}, {{item_Berries, 4}}},

        // platform
        { {item_Main_Platform, 4}, {{item_Main_WoodPlanks, 1}}},

        { {item_Background_WoodPlanks, 4}, {{item_Main_WoodPlanks, 1}}},

        // stone brick
        { {item_Main_StoneBricks, 1}, {{item_Main_Stone}}},
        { {item_Main_StonePlate, 1}, {{item_Main_Stone}}},
        { {item_Main_StonePlateCracked, 1}, {{item_Main_Stone}}},
        { {item_Background_StoneBricks, 4}, {{item_Main_StoneBricks}}},
        { {item_Background_StonePlate, 4}, {{item_Main_StonePlate}}},
        { {item_Background_StonePlateCracked, 4}, {{item_Main_StonePlateCracked}}},
    };

    const RecipeGroupData RECIPE_GROUP[rgroup_NUMBER_OF_RECIPE_GROUPS] = {

        // inventory
        {3, 1, {recipe_Torch, recipe_Platform, recipe_Workbench }},
        // workbench
        {5, 1, {recipe_Furnace, recipe_Anvil, recipe_Chest, recipe_Cookpot, recipe_StoneCutter}},
        // furnace
        {3, 1, {recipe_Copper, recipe_Iron, recipe_Gold}},
        // anvil
        {3,4, {recipe_Copper_Picaxe, recipe_Copper_Hammer, recipe_Copper_Axe, recipe_Copper_Sword, recipe_Iron_Picaxe, recipe_Iron_Hammer, recipe_Iron_Axe, recipe_Iron_Sword, recipe_Gold_Picaxe, recipe_Gold_Hammer, recipe_Gold_Axe, recipe_Gold_Sword}},
        // cook pot
        {1,1, {recipe_BerryHandful}},
        // stone cutter
        {3 , 2, {recipe_StoneBricks, recipe_Background_StoneBricks, recipe_StonePlate, recipe_Background_StonePlate, recipe_StonePlateCracked, recipe_Background_StonePlateCracked}},
    };

    const ItemData ITEM_DATA[item_NUMBER_OF_ITEMS]{

        // name, type, index within type

        {}, // item_NO_DROP

        // main
        { "Dirt", type_Main, main_Dirt, isprite_Dirt, sound_Rubble},
        { "Stone", type_Main, main_Stone, isprite_Stone, sound_Rubble},

        { "Stone Bricks", type_Main, main_Stone_Bricks, isprite_StoneBrick, sound_Rubble },
        { "Stone Plate", type_Main, main_Stone_Plate, isprite_StonePlate, sound_Rubble },
        { "Cracked Stone Plate", type_Main, main_Stone_Plate_Cracked, isprite_StonePlateCracked, sound_Rubble },

        { "Clay", type_Main, main_Clay, isprite_Dirt, sound_Rubble },
        { "Sandstone Plate", type_Main, main_Sandstone_Plate, isprite_Dirt, sound_Rubble },

        { "Ancient Stone Plate", type_Main, main_AncientStonePlate, isprite_AncientStonePlate, sound_Rubble },
        { "Cracked Ancient Stone Plate", type_Main, main_AncientStonePlateCracked, isprite_AncientStonePlateCracked, sound_Rubble },
        { "Shaped Ancient Stone Plate", type_Main, main_AncientStonePlateShaped, isprite_AncientStonePlateShaped, sound_Rubble },
        { "Chiseled Ancient Stone Plate", type_Main, main_AncientStonePlateChiseled, isprite_AncientStonePlateChiseled, sound_Rubble },
        { "Ancient Stone Bricks", type_Main, main_AncientStoneBricks, isprite_AncientStoneBricks, sound_Rubble },

        { "Wood Planks", type_Main, main_WoodPlanks, isprite_WoodPlanks, sound_Rubble},

        { "Raw Copper", type_Main, main_CopperOre, isprite_CopperOre, sound_Rubble },
        { "Raw Iron", type_Main, main_IronOre, isprite_IronOre, sound_Rubble},
        { "Raw Gold", type_Main, main_GoldOre, isprite_GoldOre, sound_Rubble},

        { "Wooden Platform", type_Main, main_Platform, isprite_Platform, sound_Rubble},

        { "Copper", type_Resource, 0, isprite_CopperBar, sound_Metal },
        { "Iron", type_Resource, 0, isprite_IronBar, sound_Metal},
        { "Gold", type_Resource, 0, isprite_GoldBar, sound_Metal},

        // background
        { "Dirt Background", type_Background, background_Dirt, isprite_Dirt_Wall, sound_Rubble},
        { "Stone Background", type_Background, background_Stone, isprite_Stone_Wall, sound_Rubble},
        { "Wood Planks Background", type_Background, background_WoodPlanks, isprite_WoodPlank_Wall, sound_Rubble},
        { "Stone Bricks Background", type_Background, background_StoneBricks, isprite_StoneBrick_Wall, sound_Rubble},
        { "Stone Plate Background", type_Background, background_StonePlate, isprite_StonePlate_Wall, sound_Rubble},
        { "Stone Plate Cracked Background", type_Background, background_StonePlateCracked, isprite_StonePlateCracked_Wall, sound_Rubble},

        // utility
        { "Hibernation Station", type_Utility, utility_Hibernator },
        { "Crafting Stool", type_Utility, utility_CraftingStool },
        { "Workbench", type_Utility, utility_WorkBench },
        { "Furnace", type_Utility, utility_Furnace },
        { "StoneCutter", type_Utility, utility_StoneCutter },
        { "Chest", type_Utility, utility_Chest },
        { "Anvil", type_Utility, utility_Anvil },
        { "Cook Pot", type_Utility, utility_Cookpot},

        // picaxe
        { "Copper Picaxe", type_Picaxe, picaxe_Copper, isprite_Copper_Picaxe, sound_Metal },
        { "Iron Picaxe", type_Picaxe, picaxe_Iron, isprite_Iron_Picaxe, sound_Metal },
        { "Gold Picaxe", type_Picaxe, picaxe_Gold, isprite_Gold_Picaxe, sound_Metal },

        // hammer
        { "Copper Hammer", type_Hammer, hammer_Copper, isprite_Copper_Hammer, sound_Metal },
        { "Iron Hammer", type_Hammer, hammer_Iron, isprite_Iron_Hammer, sound_Metal },
        { "Gold Hammer", type_Hammer, hammer_Gold, isprite_Gold_Hammer, sound_Metal },

        // sword
        { "Copper Sword", type_Hammer, hammer_Copper, isprite_Copper_Sword, sound_Metal },
        { "Iron Sword", type_Hammer, hammer_Iron, isprite_Iron_Sword, sound_Metal },
        { "Gold Sword", type_Hammer, hammer_Gold, isprite_Gold_Sword, sound_Metal },

        // axe
        { "Copper Axe", type_Axe, axe_Copper, isprite_Copper_Axe, sound_Metal },
        { "Iron Axe", type_Axe, axe_Iron, isprite_Iron_Axe, sound_Metal },
        { "Gold Axe", type_Axe, axe_Gold, isprite_Gold_Axe, sound_Metal },


        { "Fibre", type_Resource, 0, isprite_Fibre, sound_Soft},
        { "Spade Leaf", type_Resource, 0, isprite_BigLeaf, sound_Soft},
        { "Book", type_Resource, 0, isprite_Book, sound_Soft},

        {"Torch", type_Foreground, foreground_Torch, isprite_Torch, sound_Soft},

        {"Sticks", type_Resource, 0, isprite_Sticks, sound_Soft},
        
        {"Mushroom", type_Food, food_Mushroom, isprite_Mushroom, sound_Soft},
        {"Berries", type_Food, food_Berries, isprite_Berries, sound_Soft},
        {"Berry Handful", type_Food, food_BerryHandful, isprite_BerryHandful, sound_Soft},

        {"Wooden Ladder", type_Main, main_WoodLadder, isprite_WoodLadder, sound_Soft},
        
    };


    // helper function to fetch the appropriate sprite for a specific item (since different block types are spread over different textures)
    inline void SetItemSprite(sf::Sprite& sprite, ItemCode item_index, bool get_inventory_sprite = true){

        const ItemData* item = &ItemDictionary::ITEM_DATA[item_index];
        sf::Texture* new_texture;


        if(!get_inventory_sprite || item->inventory_sprite == isprite_USE_MAIN_SPRITE){
            switch(item->type){


                case ItemType::type_Main:{

                    new_texture = AssetManager::GetTexture("tiles");

                    int tex_x = item->code_in_type % main_tiles_sprites_per_row; 
                    int tex_y = floor(item->code_in_type / (float)main_tiles_sprites_per_row);

                    sprite.setTextureRect(sf::IntRect(tex_x * tile_size, tex_y * tile_size, tile_size, tile_size));            
                    
                    break;
                }

                case ItemType::type_Utility:{
                
                    new_texture = AssetManager::GetTexture("utility_tiles");

                    UtilityBlockData utility_data = UTILITY_BLOCK_DATA[item->code_in_type];

                    // sets texture rect according to footprint
                    sprite.setTextureRect(sf::IntRect(utility_data.texture_coordinate.x * 8,
                                                    utility_data.texture_coordinate.y * 8,
                                                    utility_data.footprint.x * 8,
                                                    utility_data.footprint.y * 8));

                    break;
                }

            }
        }
        else{
            new_texture = AssetManager::GetTexture("inventory_sprites");

            int tex_x = item->inventory_sprite % inventory_sprites_per_row; 
            int tex_y = floor(item->inventory_sprite / (float)inventory_sprites_per_row);

            sprite.setTextureRect(sf::IntRect(tex_x * inventory_item_tile_size, tex_y * inventory_item_tile_size, inventory_item_tile_size, inventory_item_tile_size));            
        }

        if(new_texture != nullptr && sprite.getTexture() != new_texture){
            sprite.setTexture(*new_texture);
        }
        if(get_inventory_sprite){
            sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().width / 2.0f, sprite.getTextureRect().height / 2.0f));
        }
    }   

    // select a random index from the provided IndexPool 
    inline signed_byte GetIndexFromPool(IndexPool pool_to_pick){
        return INDEX_POOLS[pool_to_pick][rand() % INDEX_POOLS[pool_to_pick].size()];
    }

    // players inventory sound of corrosponding item code
    inline void PlayInventorySound(ItemCode item, float pitch_offset = 0){
        Sound::Play(ItemDictionary::INVENTORY_SOUNDS[ItemDictionary::ITEM_DATA[item].inventory_sound].c_str(), 15.0f, 1.0f + pitch_offset);
    }
}

