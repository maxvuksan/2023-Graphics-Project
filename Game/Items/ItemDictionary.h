#pragma once
#include <SFML/Graphics.hpp>
#include "../../Amber/Core/AssetManager.h"
#include "Block.h"
#include "Sprites.h"
#include "Recipes.h"
#include <math.h>


/*

    Idea : 

    Fridge, prevents food expiration
*/

struct BlockData{

    int durability = 100;
    sf::Color base_colour = sf::Color(0,0,0); // used for minimap
    
    ItemCode pickup; // what item it drops
};

struct ItemData{
    
    std::string name;
    ItemType type;
    int code_in_type; // code within specific item type bracket
    SpriteInventory inventory_sprite = isprite_USE_MAIN_SPRITE;
};

struct UtilityBlockData{
    BlockData block_data;


    sf::Vector2i footprint; // how many tiles the object takes up
    sf::Vector2i texture_coordinate; // which coordinate on the texture is it, assuming the texture follows an 8px grid
};

namespace ItemDictionary {

    const int tile_size = 8;
    const int inventory_item_tile_size = 16;
    
    const int main_tiles_sprites_per_row = 6;
    const int inventory_sprites_per_row = 12;

    const short TYPE_STACK_SIZES[type_NUMBER_OF_TYPES] = {
        16,      //type_Main,
        16,       //type_Foreground,
        16,        //type_Background,
        16,     //type_Utility,
        1,      //type_Picaxe,
        1,      //type_Hammer,
        16,
    };

    const BlockData MAIN_BLOCK_DATA[main_NUMBER_OF_BLOCKS]{

        {6, sf::Color(109,62,62), item_Main_Dirt}, 
        {8, sf::Color(87,81,107), item_Main_Stone},

        {12, sf::Color(135,135,161), item_Main_StoneBricks},
        {12, sf::Color(135,135,161), item_Main_StonePlate},
        {12, sf::Color(135,135,161), item_Main_StonePlateCracked},

        {12, sf::Color(179,135,94), item_Main_SandStone},
        {12, sf::Color(179,135,94), item_Main_SandStonePlate},

        {12, sf::Color(171,110,83), item_Copper},
        {14, sf::Color(199,144,120), item_Iron},
        {16, sf::Color(197,151,90), item_Gold}
    };

    const BlockData FOREGROUND_BLOCK_DATA[foreground_NUMBER_OF_BLOCKS]{

        {0, sf::Color::Transparent, item_Fibre},
        {0, sf::Color::Transparent, item_NO_DROP},
        {0, sf::Color(73, 109, 8), item_NO_DROP},
    };

    const BlockData BACKGROUND_BLOCK_DATA[background_NUMBER_OF_BLOCKS]{
        
        {6, sf::Color(89,42,42), item_Background_Dirt},
        {8, sf::Color(67,61,87), item_Background_Stone},
    };

    const UtilityBlockData UTILITY_BLOCK_DATA[utility_NUMBER_OF_BLOCKS]{
        {{5, sf::Color(50,50,50), item_Utility_CraftingStool}, sf::Vector2i(1,1), sf::Vector2i(0,0)},
        {{5, sf::Color(50,50,50), item_Utility_WorkBench}, sf::Vector2i(2,2), sf::Vector2i(1,0)},
        {{5, sf::Color(50,50,50), item_Utility_Furnace}, sf::Vector2i(2,2), sf::Vector2i(3,0)},
        {{5, sf::Color(50,50,50), item_Utility_HeavyFurnace}, sf::Vector2i(2,2), sf::Vector2i(5,0)},
        {{5, sf::Color(50,50,50), item_Utility_Chest}, sf::Vector2i(2, 2), sf::Vector2i(0,2)}
    };

    const PicaxeData PICAXE_DATA[picaxe_NUMBER_OF_PICAXES]{
        {1, 2.0f}, // Copper
        {1, 2.6f}, // Iron
        {2, 13.1f} // Gold
    };

    const PicaxeData HAMMER_DATA[hammer_NUMBER_OF_HAMMERS]{
        {1, 2.0f}, // Copper
        {1, 2.6f}, // Iron
        {2, 13.1f} // Gold
    };

    const RecipeData RECIPE_DATA[recipe_NUMBER_OF_RECIPES]{
        // picaxe
        { {item_Copper_Picaxe}, {{item_Copper, 6}}  },
        { {item_Iron_Picaxe}, {{item_Iron, 6}}  },
        { {item_Gold_Picaxe}, {{item_Gold, 6}}  },

        // hammer
        { {item_Copper_Hammer}, {{item_Copper, 6}}  },
        { {item_Iron_Hammer}, {{item_Iron, 6}}  },
        { {item_Gold_Hammer}, {{item_Gold, 6}}  },

        // sword
        { {item_Copper_Sword}, {{item_Copper, 6}}  },
        { {item_Iron_Sword}, {{item_Iron, 6}}  },
        { {item_Gold_Sword}, {{item_Gold, 6}}  }
    };

    const RecipeGroupData RECIPE_GROUP[rgroup_NUMBER_OF_RECIPE_GROUPS] = {
        {3,3, {{recipe_Copper_Picaxe, recipe_Copper_Hammer, recipe_Copper_Sword}, {recipe_Iron_Picaxe, recipe_Iron_Hammer, recipe_Iron_Sword}, {recipe_Gold_Picaxe, recipe_Gold_Hammer, recipe_Gold_Sword}}}
    };

    const ItemData ITEM_DATA[item_NUMBER_OF_ITEMS]{

        // name, type, index within type

        {}, // item_NO_DROP

        // main
        { "Dirt", type_Main, main_Dirt, isprite_Dirt},
        { "Stone", type_Main, main_Stone, isprite_Stone},

        { "Stone Bricks", type_Main, main_Stone_Bricks, isprite_StoneBrick },
        { "Stone Plate", type_Main, main_Stone_Plate, isprite_StonePlate },
        { "Cracked Stone Plate", type_Main, isprite_StonePlateCracked },

        { "Sandstone", type_Main, main_Sandstone, isprite_Dirt },
        { "Sandstone Plate", type_Main, main_Sandstone_Plate, isprite_Dirt },

        { "Copper", type_Main, main_Copper, isprite_CopperBar },
        { "Iron", type_Main, main_Iron, isprite_IronBar},
        { "Gold", type_Main, main_Gold, isprite_GoldBar},

        // background
        { "Dirt Background", type_Background, background_Dirt, isprite_StoneBrick_Wall },
        { "Stone Background", type_Background, background_Stone, isprite_StoneBrick_Wall },


        // utility
        { "Crafting Stool", type_Utility, utility_CraftingStool },
        { "Workbench", type_Utility, utility_WorkBench },
        { "Furnace", type_Utility, utility_Furnace },
        { "Heavy Furnace", type_Utility, utility_HeavyFurnace },
        { "Chest", type_Utility, utility_Chest },

        // picaxe
        { "Copper Picaxe", type_Picaxe, picaxe_Copper, isprite_Copper_Picaxe },
        { "Iron Picaxe", type_Picaxe, picaxe_Iron, isprite_Iron_Picaxe},
        { "Gold Picaxe", type_Picaxe, picaxe_Gold, isprite_Gold_Picaxe },

        // hammer
        { "Copper Hammer", type_Hammer, hammer_Copper, isprite_Copper_Hammer },
        { "Iron Hammer", type_Hammer, hammer_Iron, isprite_Iron_Hammer},
        { "Gold Hammer", type_Hammer, hammer_Gold, isprite_Gold_Hammer },

        // sword
        { "Copper Sword", type_Hammer, hammer_Copper, isprite_Copper_Sword },
        { "Iron Sword", type_Hammer, hammer_Iron, isprite_Iron_Sword},
        { "Gold Sword", type_Hammer, hammer_Gold, isprite_Gold_Sword },

        { "Fibre", type_Resource, picaxe_Copper, isprite_Fibre},
        { "Spade Leaf", type_Resource, picaxe_Copper, isprite_BigLeaf},
        { "Book", type_Resource, picaxe_Copper, isprite_Book},
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
}

