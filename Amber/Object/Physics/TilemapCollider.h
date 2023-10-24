#pragma once
#include "../Component.h"
#include "BoxCollider.h"

class Tilemap;
class TilemapCollider : public BoxCollider {

    /*
        An extension of BoxCollider, providing extra functionality for use
        in unison with a Tilemap
    */

    public:
        TilemapCollider();
        void Update() override;

        // assigns which tilemap should be used to generate collider rects
        void SetTilemap(Tilemap* tilemap);

        /*
            is a position within the bounds of the tilemap
        */
        bool WithinBoundsX(float x);
        bool WithinBoundsY(float y);

        void Reset();

    private:
        /*
            for each non-empty tile, adds a rectangle 
        */
        void CreateColliders();
        /*
            equivelant to CreateColliders in a more optimized manner, joining adjacent rectangles where possible
        */
        void CreateCollidersOptimized();

        void CreateCollidersOptimized_JoinRows(std::vector<std::vector<Rect>>& blocks, sf::Vector2i tile_size);
        void CreateCollidersOptimized_FilterAndAddReferences(std::vector<std::vector<Rect*>>& block_refs, sf::Vector2i tile_size);
    
        bool reset;
        Tilemap* tilemap;
        
        

};