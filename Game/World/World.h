#pragma once
#include "../../Amber/Framework.h"
#include "WorldSettings.h"
#include "../BlockTypes.h"
#include "Minimap.h"
#include "WorldGenerator.h"
#include "Chunk.h"
#include "WorldProfile.h"

class GameClient;
class WorldGenerator;
class World : public Object{
    
    friend class WorldGenerator;

    public:

        void LinkClient(GameClient* client);

        void Start() override;
        void Update() override;

        // iterates over every chunk recalculating the minimap
        void CalculateMinimap();

        /*
            converts a world position to the appropriate chunks SetTile call
            @returns true if the tile could be set, false otherwise
        */
        bool SetTileWorld(short tile_index, int world_x, int world_y, SetLocation set_location = SetLocation::FOREGROUND, bool send_packet = false);
        bool SetTile(short tile_index, int x, int y, SetLocation set_location = SetLocation::FOREGROUND, SetMode set_mode = SetMode::OVERRIDE, bool send_packet = false);
        
        // same as SetTileWorld to -1, but also spawns pickup if needed
        bool BreakTileWorld(int world_x, int world_y, SetLocation set_location = SetLocation::FOREGROUND, bool send_packet = false);

        // @returns the tile_index at a specific world position
        short GetTileWorld(int world_x, int world_y, SetLocation set_location = SetLocation::FOREGROUND);
        // @returns the tile_index at a specific coordinate
        short GetTile(int x, int y, SetLocation set_location = SetLocation::FOREGROUND);

        // converting world space coordinates to chunk relative and tilemap offsets
        bool ChunkInBounds(int chunk_x, int chunk_y);
        // @returns a world position rounded by increments of the tilesize
        sf::Vector2i RoundWorld(int world_x, int world_y);
        // @returns the coordinate position of a world position
        sf::Vector2i WorldToCoord(int world_x, int world_y);
        // @returns the world position of a coordinate
        sf::Vector2i CoordToWorld(int x, int y);
        // @returns the chunk the provided position falls in
        sf::Vector2i ChunkFromCoord(int x, int y);
        // @returns the tilemap position of a coordinate, relative to the provided chunk
        sf::Vector2i OffsetFromCoord(int x, int y, int chunk_x, int chunk_y);

        // given a radius, will return all the tile positions in said radius ( assuming the origin is (0,0) )
        // can later be stored and reusued for circular drawing
        std::vector<sf::Vector2i> CalculateOffsetsInRadius(int radius);
        std::vector<sf::Vector2i> GetOffsetsInRadius(int radius);
        void SetCircle(short tile_index, int x, int y, int radius, SetLocation set_location = SetLocation::FOREGROUND, SetMode set_mode = SetMode::OVERRIDE);

        //the focus is what the world orients around (only load chunks around the focus transform, etc...)
        void SetFocus(Transform* focus);

        WorldProfile* GetWorldProfile(){return &world_profile;}

        std::vector<std::vector<Chunk*>>* GetChunks(){ return &chunks; }

    private:
        GameClient* client;
        Transform* focus;

        // storing tiles within specified radii, calculating via CalculateOFfsetsInRadius()
        std::unordered_map<int, std::vector<sf::Vector2i>> radius_offsets;

        std::vector<std::vector<Chunk*>> chunks;
        
        Minimap* minimap;
        WorldGenerator* generator;
        WorldProfile world_profile;    
        TilemapProfile* tilemap_profile;

        int loading_threshold = 500;
        int collider_threshold = 300;


        int half_tilemap_width;
        int half_tilemap_height;
        float one_divide_tilemap_width;
        float one_divide_tilemap_height;
};