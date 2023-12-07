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

        virtual void OnSetActive() override;
        virtual void OnDisable() override;

        void Draw(sf::RenderTarget& surface);

        Tilemap* GetTilemap(SetLocation set_location);

        bool loaded_in_scene;
        bool dirty; // has a block been updated in this chunk?

        template <typename T>
        T* AddObjectToChunk(int render_layer = 0){
            T* obj = Memory::New<T>(__FUNCTION__);
            
            obj->LinkScene(GetScene());
            obj->SetRenderLayer(render_layer);
            obj->Start();
            objects_bound_to_chunk.push_back(obj);

            return obj;

        }

        template <typename T>
        T* AddUIToChunk(int render_layer = 0){
            T* obj = Memory::New<T>(__FUNCTION__);
            
            obj->SetUI(true);
            obj->LinkScene(GetScene());
            obj->SetRenderLayer(render_layer);
            obj->Start();
            ui_bound_to_chunk.push_back(obj);

            return obj;
        }

        std::vector<Object*>* GetThisObjectsInChunk() { return &objects_bound_to_chunk;}
        std::vector<Object*>* GetUIInChunk() { return &ui_bound_to_chunk;}

        ~Chunk();

        sf::Image& GetLightmap(){return light_map; }
        void MarkLightmapDirty(){light_map_dirty = true;}

    private:

        std::vector<Object*> objects_bound_to_chunk;
        std::vector<Object*> ui_bound_to_chunk;

        sf::Image light_map;
        sf::Texture light_texture;
        bool light_map_dirty;

        TilemapProfile* tilemap_profile;
        Tilemap* foreground_tilemap;
        Tilemap* main_tilemap;
        Tilemap* background_tilemap;

};