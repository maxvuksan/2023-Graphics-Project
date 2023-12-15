#include "../../Amber/Object/Rendering/TilemapProfile.h"

struct WorldProfile{

    int width = 50;
    int height = 50;

    TilemapProfile tilemap_profile = {
        8,8,
        50,50
    };

    unsigned int width_in_tiles; // calculated by world
    unsigned int height_in_tiles; // calculated by world
}; 