#include <SFML/Graphics.hpp>
#pragma once

enum BlockCode{
    
    // code _ name

    c_Stone,
    c_Dirt,
    
    c_Stone_Bricks,
    c_Stone_Plate,
    c_Stone_Stylized,
    c_Stone_Chizeled,
    c_Stone_Extruded,

    c_Copper,
    c_Iron,
    c_Gold,

    c_NUMBER_OF_BLOCKS,
};

struct BlockData{

    int durability = 100;
    sf::Color base_colour = sf::Color(0,0,0); // used for minimap
};


const BlockData Blocks[c_NUMBER_OF_BLOCKS]{

    {6, sf::Color(87,81,107)},
    {8, sf::Color(109,62,62)},

    {12, sf::Color(135,135,161)},
    {12, sf::Color(135,135,161)},
    {12, sf::Color(135,135,161)},
    {12, sf::Color(135,135,161)},
    {12, sf::Color(135,135,161)},

    {12, sf::Color(171,110,83)},
    {14, sf::Color(199,144,120)},
    {16, sf::Color(197,151,90)},
};

