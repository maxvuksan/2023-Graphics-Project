#pragma once

/*
    Aims to reduce duplicated variables. In the case of a chunk based world where each tilemap 
    would have the same configuration 
*/
struct TilemapProfile{

    // pixel size of each tile
    unsigned int tile_width = 8;
    unsigned int tile_height = 8;

    // size of the tilemap in tiles
    unsigned int width = 16;
    unsigned int height = 16;
    
};

