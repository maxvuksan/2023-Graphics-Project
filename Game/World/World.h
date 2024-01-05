#pragma once
#include "../../Amber/Framework.h"
#include "WorldSettings.h"
#include "../Items/ItemDictionary.h"
#include "Minimap.h"
#include "WorldGenerator.h"
#include "Chunk.h"
#include "WorldProfile.h"
#include "TimeManager.h"




class WorldScene;
class GameClient;
class WorldGenerator;
class World : public Object{
    
    friend class CommandParser;
    friend class WorldGenerator;
    friend class Chunk;
    friend class TileBehaviourManager;

    public:

        ~World();

        void LinkClient(GameClient* client);
        void LinkWorldScene(WorldScene* world_scene);

        /*
            initalizes all data and generates the world
        */
        void Create();
        void Update() override;

        /*
            completley recalculates the minimap, iterating over each coordinate
        */
        void CalculateMinimap();

        /*
            sets a tile through a world position

            @returns true if the tile could be set, false otherwise
            
            @param tile_index the index (within the appropriate set_location) to set
            @param world_x the x component of the world position
            @param world_y the y component of the world position
            @param set_location the layer we are interacting with
            @param send_packet should this operation send a networking packet?
        */
        bool SetTileWorld(signed_byte tile_index, float world_x, float world_y, SetLocation set_location = SetLocation::MAIN, bool send_packet = false);
        
        /*
            sets a tile through a coordinate position
            
            @returns true if the tile could be set, false otherwise
            
            @param tile_index the index (within the appropriate set_location) to set
            @param x the x component of the coordinate
            @param y the y component of the coordinate
            @param set_location the layer we are interacting with
            @param set_mode determines if the tile can be set in relation to exisiting tiles (see SetMode enum)
            @param send_packet should this operation send a networking packet?
            @param propogate_to_other_tiles does this set operation causes the TileBehaviourManager to propogate at this coordinate
            @param create_pickup should an item pickup be created from the old tile?
        */
        bool SetTile(signed_byte tile_index, int x, int y, SetLocation set_location = SetLocation::MAIN, SetMode set_mode = SetMode::OVERRIDE, bool send_packet = false, bool propogate_to_other_tiles = false, bool create_pickup = false);
        
        /*
            @param item_to_drop the item code of the pickup
            @param world_x the x component of the world position
            @param world_y the y component of the world position
        */
        void CreatePickup(ItemCode item_to_drop, float world_x, float world_y);
        
        /*
            drawing a single tile on a set layer to the minimap

            @param tile_index the index (within the appropriate set_location) to set
            @param x the x component of the coordinate
            @param y the y component of the coordinate
            @param set_location the layer we are interacting with
        */
        void DrawTileToMinimap(signed_byte tile_index, int x, int y, SetLocation set_location);

        /*
            reveals tiles on the minimap explored layer within a set radius (Minimap::exploring_radius)
        */
        void RevealMapAroundFocus(); 

        /*
            aids checking if a position is valid for placement

            @returns true if the coordinate provided has adjacent tiles (on either main tilemap or background tilemap)
            
            @param x the x component of the coordinate
            @param y the y component of the coordinate
        */
        bool CoordIsConnectedToOtherTiles(int x, int y);

        // @returns the tile_index at a specific world position
        signed_byte GetTileWorld(float world_x, float world_y, SetLocation set_location = SetLocation::MAIN);

        // @returns the tile_index at a specific coordinate
        signed_byte GetTile(int x, int y, SetLocation set_location = SetLocation::MAIN);

        // @returns a struct containing the byte for each layer
        EntireTile GetEntireTile(int x, int y);
        
        // @returns starting at world_position, returns the nearest tile coordinate in the mouses direction
        sf::Vector2i GetNearestTileInDirectionOfMouse(sf::Vector2f world_position, SetLocation set_location = SetLocation::MAIN);

        // @returns true if a chunk coordinate is valid
        bool ChunkInBounds(int chunk_x, int chunk_y);
        
        // @returns true if a coordinate is valid
        bool CoordInBounds(int x, int y);
        
        // @returns a world position rounded by increments of the tilesize
        sf::Vector2i RoundWorld(float world_x, float world_y);
        
        // @returns the coordinate position of a world position
        sf::Vector2i WorldToCoord(float world_x, float world_y);
        
        // @returns the world position of a coordinate
        sf::Vector2f CoordToWorld(int x, int y);
        
        // @returns the chunk the provided position falls in
        sf::Vector2i ChunkFromCoord(int x, int y);
        
        // @returns the tilemap position of a coordinate, relative to the provided chunk
        sf::Vector2i OffsetFromCoord(int x, int y, int chunk_x, int chunk_y);

        // given a radius, will return all the tile positions in said radius ( assuming the origin is (0,0) )
        // can later be stored and reusued for circular drawing
        std::vector<sf::Vector2i> CalculateOffsetsInRadius(int radius);
        std::vector<sf::Vector2i> GetOffsetsInRadius(int radius);
        void SetCircle(signed_byte tile_index, int x, int y, int radius, SetLocation set_location = SetLocation::MAIN, SetMode set_mode = SetMode::OVERRIDE);

        //the focus is what the world orients around (only load chunks around the focus transform, etc...)
        void SetFocus(Transform* focus);
        Transform* GetFocus(){return focus;}
        
        int GetLoadDistance(){return load_distance;}

        WorldProfile* GetWorldProfile(){return &world_profile;}
        
        std::vector<std::vector<Chunk*>>* GetChunks(){ return &chunks; }

        // @returns true if the world has had a change which requires pathfinding node grid to be recalculated, is reset to false by PathfindingGraph
        bool GetWorldNeedsPathfindingRecalculating(){return world_needs_pathfinding_recalculating; }
        void SetWorldNeedsPathfindingRecalculating(bool state);

        // @returns minimum coordinate of active chunks
        sf::Vector2i GetActiveChunksMin(){return active_chunks_min;}
        // @returns maximum coordinates of active chunks
        sf::Vector2i GetActiveChunksMax(){return active_chunks_max;}

        // @returns the coordinate of the world spawnpoint
        sf::Vector2i GetSpawnCoordinate(){return world_spawn_coord;}
    private:
        GameClient* client;
        Transform* focus;
        WorldScene* world_scene;

        // storing tiles within specified radii, calculating via CalculateOFfsetsInRadius()
        std::unordered_map<int, std::vector<sf::Vector2i>> radius_offsets;
        std::vector<std::vector<Chunk*>> chunks;
        
        sf::Vector2i world_spawn_coord;

        Minimap* minimap;
        WorldGenerator* generator;
        WorldProfile world_profile;    
        TilemapProfile* tilemap_profile;

        int load_distance = 2;
        sf::Vector2i active_chunks_min; 
        sf::Vector2i active_chunks_max; 

        bool world_needs_pathfinding_recalculating;

        int half_tilemap_width;
        int half_tilemap_height;
        float one_divide_tilemap_width;
        float one_divide_tilemap_height;
};