#pragma once
#include "../Items/Item.h"
#include "WorldSettings.h"
#include <map>
#include "../../Amber/Framework.h"

struct EntireTile{
    signed_byte main;
    signed_byte background;
    signed_byte foreground;
};

class World;
class UtilityStation;

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
        void RemoveObjectFromChunk(T* object){

            for(int i = 0; i < objects_bound_to_chunk.size(); i++){
                
                if(objects_bound_to_chunk[i] == object){
                    objects_bound_to_chunk.erase(objects_bound_to_chunk.begin() + i);
                    Memory::Delete(object, __FUNCTION__);
                    return;
                    
                }
            }

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

        /*
            adds a new foliage object at local position x, y
            
            @param foliage_index the specific foliage item to create
            @param x component of tilemap offset
            @param y component of tilemap offset
        */
        void AddFoliage(Foliage foliage_index, int x, int y);
        /*
            removes a foliage object at local position x, y
            @param x component of tilemap offset
            @param y component of tilemap offset
        */ 
        void RemoveFoliage(int x, int y);

        /*
            iterates over the chunks foliage map, constructing a vertex array with each foliage objects data,
            by having all foliage on a single texture only one draw call is necassary aiding performance
        */
        void RecalculateFoliageVertexArray();

        /*
            stores a reference of a UtilityStation in this chunk.
            saying this chunk "owns" the utility station,

            @param reference the object we wish to store

            should be called after created new utility stations 
        */
        void StoreUtilityStationReference(UtilityStation* reference);

        /*
            removes a utility station and creates any necassary item pickups through World::CreatePickup()

            @param reference the object we wish to break
        */
        void BreakUtilityStation(UtilityStation* reference);


        /*
            @returns references to all UtilityStations attached to this chunk
        */
        std::vector<UtilityStation*>* GetUtilityStations(){return &utility_stations;}

        /*
            @returns references to all objects bound to this specific chunk. 
            Is used to only simulate whatever chunks are active, 
            if this chunk is not active, the objects bound to the chunk are simply idle in RAM
        */
        std::vector<Object*>* GetThisObjectsInChunk() { return &objects_bound_to_chunk;}
        std::vector<Object*>* GetUIInChunk() { return &ui_bound_to_chunk;}

        ~Chunk();

        sf::Image& GetLightmap(){return light_map; }
        sf::Texture& GetLightmapTexture(){return light_texture;}
        void MarkLightmapDirty(){light_map_dirty = true;}

        void RemoveTorchPosition(int  coord_x, int coord_y);
        void AddTorchPosition(int coord_x, int coord_y);

        void ClearLightmap();
        void RefreshSkylight();
        void CalculateSkyLight();
    /*
        // checks if the current position has already been considered, if not add it to the sky light vectors
        void IntroduceTileToSkylight(int x, int y);
        // checks the existence of a position in the sky light vectors, if it exists, remove it. 
        void RemoveTileFromSkylight(int x, int y);
    */
        std::vector<std::vector<float>> lighting_closed_grid;

    private:

        // local positions which should emit skylight, 
        std::vector<sf::Vector2i> sky_tiles_to_propogate;
        // do not propogate these values, simply set them, they are sky values
        std::vector<sf::Vector2i> sky_tiles;

        bool foliage_map_dirty;
        sf::VertexArray foliage_vertex_array;

        // key (x position + y * width) : foliage id
        std::map<int, Foliage> foliage_map;

        std::vector<Object*> objects_bound_to_chunk;
        std::vector<Object*> ui_bound_to_chunk;

        std::vector<UtilityStation*> utility_stations; 
        std::vector<sf::Vector2i> torch_positions;

        bool skylight_first_calculated = false;

        sf::Image light_map;
        sf::Texture light_texture;
        bool light_map_dirty;

        World* world;
        Tilemap* foreground_tilemap;
        Tilemap* main_tilemap;
        Tilemap* background_tilemap;

};