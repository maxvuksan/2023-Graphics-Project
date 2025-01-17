#pragma once
#include "../Items/Item.h"
#include "WorldSettings.h"
#include <map>
#include "../../Amber/Framework.h"
#include "../Pathfinding/PathfindingGraph.h"

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

        void SetChunkCoordinate(int x, int y);
        sf::Vector2i GetChunkCoordinate(){return chunk_coordinate;}

        void ClearColliders();
        void ResetCollidersIfChanged();
        void ResetColliders();

        virtual void OnSetActive() override;
        virtual void OnDisable() override;

        // constructs a grid of nodes which can be interpretted for path finding 
        void CalculatePathfindingGrid();
        std::vector<std::vector<PathfindingNode>>& GetPathfindingGrid(){return pathfinding_nodes;}

        void DrawDebug(sf::RenderTarget& surface) override;
        void Draw(sf::RenderTarget& surface) override;
        void DrawBackgroundStructures(sf::RenderTarget& surface);
        void PropogateTorches();

        Tilemap* GetTilemap(SetLocation set_location);

        Tilemap* GetTilemapMain(){return main_tilemap;}
        Tilemap* GetTilemapBackground(){return background_tilemap;}
        Tilemap* GetTilemapForeground(){return foreground_tilemap;}

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
        void AddFoliageViaMapIndex(Foliage foliage_index, int map_index);

        void AddStructure(BackgroundStructure structure, int x, int y);
        void AddStructureViaMapIndex(BackgroundStructure structure, int map_index);

        /*
            removes a foliage object at local position x, y
            @param x component of tilemap offset
            @param y component of tilemap offset
        */ 
        void RemoveFoliage(int x, int y);
        const sf::VertexArray& GetFoliageVertexArray(){return foliage_vertex_array;}

        /*
            iterates over the chunks foliage map, constructing a vertex array with each foliage objects data,
            by having all foliage on a single texture only one draw call is necassary aiding performance
        */
        void RecalculateFoliageVertexArray();
        void RecalculateStructureVertexArray();

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
        void MarkLightmapForTextureReload(){light_map_needs_texture_reload = true;}
        void MarkSurroundingChunksLightmapDirty();
        void SetLightmapDirty(bool state){light_map_dirty = state; light_map_needs_texture_reload = true;}
        bool GetLightmapDirty(){return light_map_dirty;}

        // @returns map structured as... key (x offset + y offset * width) : foliage id
        const std::map<int, Foliage>& GetFoliageMap(){return foliage_map;}
        const std::map<int, BackgroundStructure>& GetStructureMap(){return structure_map;}

        void RecalculateTorchPositions();
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

        // saving dirty determines if a chunk has changed since it was last saved, only dirty chunks need to be scanned and written into the save file
        bool GetSavingDirty(){return saving_dirty;} 
        void SetSavingDirty(bool saving_dirty){this->saving_dirty = saving_dirty;}

        Datafile save_datafile;

    private:

        bool saving_dirty;

        std::vector<std::vector<PathfindingNode>> pathfinding_nodes;

        sf::Vector2i chunk_coordinate;

        // local positions which should emit skylight, 
        std::vector<sf::Vector2i> sky_tiles_to_propogate;
        // do not propogate these values, simply set them, they are sky values
        std::vector<sf::Vector2i> sky_tiles;

        bool foliage_map_dirty;
        bool structure_map_dirty;
        sf::VertexArray foliage_vertex_array;
        sf::VertexArray structure_vertex_array;

        // key (x position + y * width) : foliage id
        std::map<int, Foliage> foliage_map;
        std::map<int, BackgroundStructure> structure_map;

        std::vector<Object*> objects_bound_to_chunk;
        std::vector<Object*> ui_bound_to_chunk;

        std::vector<UtilityStation*> utility_stations; 
        std::vector<sf::Vector2i> torch_positions;

        bool skylight_first_calculated = false;

        sf::Image light_map;
        sf::Texture light_texture;
        bool light_map_dirty; // does this chunks lighting need to be propogated again
        bool light_map_needs_texture_reload; // are the image and texture are out of sync?

        World* world;
        Tilemap* foreground_tilemap;
        Tilemap* main_tilemap;
        Tilemap* background_tilemap;

};