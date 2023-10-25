#pragma once
#include "BlockTypes.h"
#include "WorldSettings.h"
#include "../Amber/Framework.h"

class Chunk : public Object {

    public:
        void Init(int tilemap_width, int tilemap_height, int tilesize_x, int tilesize_y);

        int GetTile(int x, int y, SetLocation get_location = SetLocation::FOREGROUND);
        void SetTile(int tile_index, int x, int y, SetLocation set_location = SetLocation::FOREGROUND);

        void ClearColliders();
        void EnableColliders();

        Tilemap* GetTilemap(SetLocation set_location);

    private:

        Object* foreground_tilemap_object; // the collision shapes
        Object* background_tilemap_object; // background elements

        Tilemap* foreground_tilemap;
        Tilemap* background_tilemap;

        int tilemap_width;
        int tilemap_height;
        int tilesize_x;
        int tilesize_y;

};