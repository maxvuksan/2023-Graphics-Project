#include "../Amber/Framework.h"
#include "WorldSettings.h"

class World : public Object{
    
    public:
        void Start() override;
        void Update() override;

                
        // shaping the terrain (hills, caves etc...)
        void SculptingPass(int chunk_x, int chunk_y, Tilemap* tilemap);
        // creating caves with direction
        void TunnelingPass();
        void Tunnel(int x, int y, int radius_min, int radius_max, float angle, float angle_step = 0);


        // given a radius, will return all the tile positions in said radius ( assuming the origin is (0,0) )
        // can later be stored and reusued for circular drawing
        std::vector<sf::Vector2i> CalculateOffsetsInRadius(int radius);
        void DrawCircle(int tile_index, int x, int y, int radius);

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

        int loading_threshold = 200;
        int collider_threshold = 150;

        int tilesize_x = 8;
        int tilesize_y = 8;

        int tilemap_width = 15;
        int tilemap_height = 15;
        int half_tilemap_width;
        int half_tilemap_height;

        // in chunks...
        int width = 25;
        int height = 20;

};