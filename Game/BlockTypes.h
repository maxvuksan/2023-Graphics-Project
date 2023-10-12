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

    c_Iron,
    c_Gold,

    c_NUMBER_OF_BLOCKS,
};

struct BlockData{

    int durability;
}