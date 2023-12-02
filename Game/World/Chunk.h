#pragma once
#include "../Items/Item.h"
#include "WorldSettings.h"
#include "../../Amber/Framework.h"

class Chunk : public Object {

    public:
        void Init(TilemapProfile* tilemap_profile, const sf::Color& background_colour);

        int GetTile(int x, int y, SetLocation get_location = SetLocation::MAIN);
        void SetTile(int tile_index, int x, int y, SetLocation set_location = SetLocation::MAIN);

        void ClearColliders();
        void ResetCollidersIfChanged();
        void ResetColliders();

        Tilemap* GetTilemap(SetLocation set_location);

        bool loaded_in_scene;
        bool dirty; // has a block been updated in this chunk?

        template <typename T>
        T* AddObjectToChunk(){
            T* obj = Memory::New<T>(__FUNCTION__);
            
            obj->LinkScene(GetScene());
            obj->Start();
            objects_bound_to_chunk.push_back(obj);

            return obj;

        }
        std::vector<Object*>* GetThisObjectsInChunk() { return &objects_bound_to_chunk;}

        ~Chunk();

    private:

        std::vector<Object*> objects_bound_to_chunk;

        Tilemap* foreground_tilemap;
        Tilemap* main_tilemap;
        Tilemap* background_tilemap;

};