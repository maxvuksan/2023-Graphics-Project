#pragma once
#include <SFML/Graphics.hpp>
#include "../Component.h"
#include "TilemapPrimitive.h"
#include "TilemapProfile.h"

/*
    An efficent way to draw grids of sprites (tiles)
*/
class Tilemap : public Component
{

    friend class TilemapCollider;

    public:

        Tilemap() : loaded(false){}
        
        void Start() override;
        ~Tilemap() override;

        void UpdateSecondary() override;
        sf::VertexArray& GetVertexArray(){return tilemap_primitive.GetVertexArray();}
        void ConstructVertexArray();
        void ClearVertexArray();

        void Draw(sf::RenderTarget& surface) override;
        void DrawSiloutte(sf::RenderTarget& surface);
        void DrawWithExternalTexture(sf::RenderTarget& surface, sf::Texture* texture);

        /*
            @param texture_label What the tile texture is saved as (in AssetManager)
            @param profile The configuration of our tilemap (note this is a referenced, the referenced object should stay in scope)
            @param tiles If we wanted to load a saved level as an array, by default will fill each space with -1
        */
        bool Load(const char* texture_label, TilemapProfile* profile, int default_tile = -1);

        signed_byte GetTile(int x, int y);
        
        void SetTile(signed_byte tile_index, unsigned int x, unsigned int y);
        // performs a bounds check to ensure tile can be set... ignores out of bounds operations, 
        // @returns true if the tile was successfully set
        bool SetTileSafe(signed_byte tile_index, int x, int y);

        int GetWidth();
        int GetHeight();
        sf::Vector2i GetTileSize();

        void SetRow(signed_byte tile_index, unsigned int row);
        void SetColumn(signed_byte tile_index, unsigned int column);
        void SetArea(signed_byte tile_index, unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max);
        void SetAll(signed_byte tile_index);

        void SetShowOverlayColour(bool show_overlay_colour);
        void SetOverlayColour(sf::Color overlay_colour);

    private:

        sf::Shader* overlay_shader;

        bool show_overlay_colour;
        sf::Color overlay_colour;
        
        bool ambient_occlusion;
        static float ambient_occlusion_blur_strength;

        bool cast_shadows;
        bool in_shadow;

        TilemapPrimitive tilemap_primitive;
        TilemapProfile* tilemap_profile;
        
        bool has_changed;
        bool loaded;

};