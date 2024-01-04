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

        void Create();
        void Update() override;

        // iterates over every chunk recalculating the minimap
        void CalculateMinimap();

        /*
            converts a world position to the appropriate chunks SetTile call
            @returns true if the tile could be set, false otherwise
        */
        bool SetTileWorld(signed_byte tile_index, float world_x, float world_y, SetLocation set_location = SetLocation::MAIN, bool send_packet = false);
        bool SetTile(signed_byte tile_index, int x, int y, SetLocation set_location = SetLocation::MAIN, SetMode set_mode = SetMode::OVERRIDE, bool send_packet = false, bool propogate_to_other_tiles = false, bool create_pickup = false);
        void CreatePickup(ItemCode item_to_drop, float world_x, float world_y);
        void DrawTileToMinimap(signed_byte tile_index, int x, int y, SetLocation set_location);
        void RevealMapAroundFocus(); 

        // @returns true if the position provided has adjacent tiles (on either main tilemap or background tilemap), aids checking if a position is valid for placement
        bool CoordIsConnectedToOtherTiles(int x, int y);

        // DEPRECATED! use SetTile with the create pickup flag = true
        // same as SetTileWorld to -1, but also spawns pickup if needed.
        bool BreakTileWorld(float world_x, float world_y, SetLocation set_location = SetLocation::MAIN, bool send_packet = false);

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