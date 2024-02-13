#include "../../Amber/Object/Rendering/TilemapProfile.h"

struct WorldProfile{

    int width = 30;
    int height = 50;

    TilemapProfile tilemap_profile = {
        8,8,
        TILEMAP_WIDTH,TILEMAP_HEIGHT
    };

    unsigned int width_in_tiles; // calculated by world
    unsigned int height_in_tiles; // calculated by world
}; 