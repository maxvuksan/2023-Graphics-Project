#include <SFML/Graphics.hpp>
#pragma once

/*
    some blocks will share pickup sprites

    e.g. all variants of Stone Brick will have the same pickup
*/
enum PickupSprites{
    
    s_Dirt,
    s_Stone,
    s_StoneBrick,
    s_Sand,
    s_Copper,
    s_Iron,
    s_Gold,

    s_NUMBER_OF_PICKUP_SPRITES
};

struct ItemData{

    int durability = 100;
    sf::Color base_colour = sf::Color(0,0,0); // used for minimap
    
    PickupSprites pickup_sprite; // what it looks like on the ground

    bool placable = true; // is it a block?
};

enum ItemCode{
    
    // code _ name
    
    c_Dirt,
    c_Stone,
    
    c_Stone_Bricks,
    c_Stone_Plate,
    c_Stone_Plate_Cracked,

    c_Sandstone,
    c_Sandstone_Plate,

    c_Copper,
    c_Iron,
    c_Gold,

    c_NUMBER_OF_ITEMS,
};

const ItemData Items[c_NUMBER_OF_ITEMS]{

    {6, sf::Color(109,62,62), s_Dirt},
    {8, sf::Color(87,81,107), s_Stone},

    {12, sf::Color(135,135,161), s_StoneBrick},
    {12, sf::Color(135,135,161), s_StoneBrick},
    {12, sf::Color(135,135,161), s_StoneBrick},

    {12, sf::Color(179,135,94), s_Sand},
    {12, sf::Color(179,135,94), s_Sand},

    {12, sf::Color(171,110,83), s_Copper},
    {14, sf::Color(199,144,120), s_Iron},
    {16, sf::Color(197,151,90), s_Gold}

};

