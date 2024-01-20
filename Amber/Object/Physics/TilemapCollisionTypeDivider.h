#pragma once
/* 
    allows one tilemap collider to have multiple collider types depending on the tiles within the map
    uses a division approach

    regular collision | platform collision 
*/
class TilemapCollisionTypeDivider{

    public:
        // any tile index greater or equal to this value will be interpreted as platforms rather than solid blocks
        static int platform_collider_begins_at;
};