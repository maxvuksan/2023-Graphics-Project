#pragma once
#include <SFML/Graphics.hpp>
#include "../Component.h"
#include "TilemapPrimitive.h"

class Tilemap : public Component
{
    /*
        Tilemap:

        An efficent way to draw grids of sprites 
    
    */

    struct Edge{
        sf::Vector2f start;
        sf::Vector2f end;

        int id = -1;

    };


    public:

        Tilemap() : loaded(false){}
        
        void Start() override;
        void OnDestroy() override;

        void UpdateSecondary() override;
        void Draw(sf::RenderTarget& surface) override;
        void Draw_ShadowPass(sf::RenderTarget& surface) override;
        void Draw_ShadowPass_PostBlur(sf::RenderTarget& surface) override;
        void Draw_Debug(sf::RenderTarget& surface) override;

        void Draw_EdgeLighting(sf::RenderTarget& surface);

        void ClearEdges();
        void CalculateEdges();
        /*
            @param texture_label What the tile texture is saved as (in AssetManager)
            @param tile_width Pixel width of each tile on texture
            @param tile_height Pixel height of each tile on texture
            @param width The allocated width of the tilemap
            @param height The allocated height of the tilemap

            @param tiles If we wanted to load a saved level as an array, by default will fill each space with -1
        */
        bool Load(const char* texture_label, unsigned int tile_width, unsigned int tile_height, unsigned int width, unsigned int height, int default_tile = -1);

        // determines if the given position should have a corner marked in the top left
        bool TileConfigurationNeedsCorner(unsigned int x, unsigned y);

        sf::Vector2i GetTileSize();
        sf::Vector2i GetSize();
        int GetWidth();
        int GetHeight();

        int GetTile(int x, int y);
        
        void SetTile(int tile_index, unsigned int x, unsigned int y);
        // performs a bounds check to ensure tile can be set... ignores out of bounds operations, 
        // @returns true if the tile was successfully set
        bool SetTileSafe(int tile_index, int x, int y);

        void SetRow(int tile_index, unsigned int row);
        void SetColumn(int tile_index, unsigned int column);
        void SetArea(int tile_index, unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max);
        void SetAll(int tile_index);
    private:

        sf::Vector2i size;
        sf::Vector2i tile_size;

        bool cast_shadows;
        bool in_shadow;

        sf::Texture* shadow_texture;
        sf::Texture* white_texture;
        sf::Texture* black_texture;

        TilemapPrimitive tilemap_primitive;
        
        sf::RenderTexture shadow_render_texture;
        sf::RenderTexture single_light_render_texture;

        std::vector<Edge> edges;

        bool has_changed;
        bool loaded;

};