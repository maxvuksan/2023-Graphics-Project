#include "../Amber/Framework.h"
#include "WorldSettings.h"

class World : public Object{
    
    public:
        void Start() override;
        void Update() override;

        /*
            converts a world position to the appropriate chunks SetTile call
            @returns true if the tile could be set, false otherwise
        */
        bool SetTile_FromWorld(int tile_index, int world_x, int world_y);
                
        // shaping the terrain (hills, caves etc...)
        void SculptingPass(int chunk_x, int chunk_y, Tilemap* tilemap);
        // creating caves with direction
        void TunnelingPass();
        void Tunnel(int x, int y, int radius_min, int radius_max, float angle, float angle_step = 0);

        // converting world space coordinates to chunk relative and tilemap offsets
        bool ChunkInBounds(int chunk_x, int chunk_y);
        sf::Vector2i ChunkFromCoord(int x, int y);
        sf::Vector2i OffsetFromCoord(int x, int y, int chunk_x, int chunk_y);

        // given a radius, will return all the tile positions in said radius ( assuming the origin is (0,0) )
        // can later be stored and reusued for circular drawing
        std::vector<sf::Vector2i> CalculateOffsetsInRadius(int radius);
        void SetCircle(int tile_index, int x, int y, int radius);

        //the focus is what the world orients around (only load chunks around the focus transform, etc...)
        void SetFocus(Transform* focus);

    private:
        Transform* focus;

        // storing tiles within specified radii, calculating via CalculateOFfsetsInRadius()
        std::unordered_map<int, std::vector<sf::Vector2i>> radius_offsets;

        std::vector<std::vector<Object*>> chunks;
        

        WorldSettings settings;
        

        PerlinNoise::seed_type seed; 
        PerlinNoise perlin;

        int loading_threshold = 300;
        int collider_threshold = 150;

        int tilesize_x = 8;
        int tilesize_y = 8;

        int tilemap_width = 35;
        int tilemap_height = 15;

        int half_tilemap_width;
        int half_tilemap_height;
        float one_divide_tilemap_width;
        float one_divide_tilemap_height;

        // in chunks...
        int width = 6;
        int height = 20;

};