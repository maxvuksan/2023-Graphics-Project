#include "../Amber/Framework.h"
#include "WorldSettings.h"
#include "Minimap.h"
#include "BlockTypes.h"

class World : public Object{
    
    public:
        void Start() override;
        void Update() override;

        // iterates over every chunk recalculating the minimap
        void CalculateMinimap();

        /*
            converts a world position to the appropriate chunks SetTile call
            @returns true if the tile could be set, false otherwise
        */
        bool SetTile_FromWorld(int tile_index, int world_x, int world_y);
        bool SetTile(int tile_index, int x, int y);
        // @returns the tile_index at a specific world position
        int GetTile_World(int world_x, int world_y);
        // @returns the tile_index at a specific coordinate
        int GetTile(int x, int y);

        // shaping the terrain (hills, caves etc...)
        void SculptingPass(int chunk_x, int chunk_y, Tilemap* tilemap);
        // creating caves with direction
        void TunnelingPass();
        void Tunnel(int x, int y, int radius_min, int radius_max, float angle, float angle_step = 0);

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
        void SetCircle(int tile_index, int x, int y, int radius);

        //the focus is what the world orients around (only load chunks around the focus transform, etc...)
        void SetFocus(Transform* focus);

    private:
        Transform* focus;

        // storing tiles within specified radii, calculating via CalculateOFfsetsInRadius()
        std::unordered_map<int, std::vector<sf::Vector2i>> radius_offsets;

        std::vector<std::vector<Object*>> chunks;
        

        WorldSettings settings;
        Minimap* minimap;

        PerlinNoise::seed_type seed; 
        PerlinNoise perlin;

        int loading_threshold = 300;
        int collider_threshold = 150;

        int tilesize_x = 8;
        int tilesize_y = 8;

        int tilemap_width = 15;
        int tilemap_height = 15;

        int half_tilemap_width;
        int half_tilemap_height;
        float one_divide_tilemap_width;
        float one_divide_tilemap_height;

        // in chunks...
        int width = 8;
        int height = 15;

};