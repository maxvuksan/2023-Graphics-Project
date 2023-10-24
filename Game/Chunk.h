#pragma once
#include "../Amber/Framework.h"

class Chunk : public Object {

    public:
        void Init(int tilemap_width, int tilemap_height, int tilesize_x, int tilesize_y);

        Tilemap* main_tilemap; // the collision shapes
        Tilemap* background_tilemap; // background elements

    private:
        
        int tilemap_width;
        int tilemap_height;
        int tilesize_x;
        int tilesize_y;

};