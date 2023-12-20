#pragma once
#include "../Items/Item.h"
#include "WorldSettings.h"
#include "../../Amber/Framework.h"

struct EntireTile{
    signed_byte main;
    signed_byte background;
    signed_byte foreground;
};

class World;
class Chunk : public Object {

    public:
        void Init(World* world);

        signed_byte GetTile(int x, int y, SetLocation get_location = SetLocation::MAIN);
        EntireTile GetEntireTile(int x, int y);
        void SetTile(signed_byte tile_index, int x, int y, SetLocation set_location = SetLocation::MAIN);

        void ClearColliders();
        void ResetCollidersIfChanged();
        void ResetColliders();

        virtual void OnSetActive() override;
        virtual void OnDisable() override;

        void Draw(sf::RenderTarget& surface);
        void PropogateTorches();

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
        sf::Texture& GetLightmapTexture(){return light_texture;}
        sf::Image& GetSkylightImage(){return skylight_image;}
        sf::Texture& GetSkylightTexture(){return skylight_texture;}
        void MarkLightmapDirty(){light_map_dirty = true;}
        void MarkSkylightDirty(){skylight_map_dirty = true;}
        std::vector<std::vector<byte>>& GetSkylightmap() {return skylight_map;}

        void RemoveTorchPosition(int  coord_x, int coord_y);
        void AddTorchPosition(int coord_x, int coord_y);

        void ClearLightmap();
        void RefreshSkylight();
        void CalculateSkyLight();


        std::vector<std::vector<float>> lighting_closed_grid;

    private:


        std::vector<Object*> objects_bound_to_chunk;
        std::vector<Object*> ui_bound_to_chunk;
        std::vector<sf::Vector2i> torch_positions;

        std::vector<std::vector<byte>> skylight_map; 
        sf::Image skylight_image;
        sf::Texture skylight_texture;
        bool skylight_first_calculated = false;
        bool skylight_map_dirty;

        sf::Image light_map;
        sf::Texture light_texture;
        bool light_map_dirty;

        World* world;
        Tilemap* foreground_tilemap;
        Tilemap* main_tilemap;
        Tilemap* background_tilemap;

};