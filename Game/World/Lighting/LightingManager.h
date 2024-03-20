#include "../Minimap.h"
#include "LightSource.h"

class World;
class Chunk;
class LightingManager : public Object{

    friend class BackgroundShadowManager;
    friend class BackgroundFogLayer;
    
    public:

        static void LinkWorld(World* world);

        /*
            propogates a light source on the grid of the world 

            @param coordinate of the light source
            @param colour to tint the light
            @param decay the amount a channel drops off each step
            @param decay_dropoff is subtracted from the decay each step
            @param decay_min the minimum a decay can reach
        */
        static void PropogateLighting(sf::Vector2i coordinate, const sf::Color& colour, int decay_step, int decay_dropoff = 2, int decay_min = 3);

        void Update() override;

        // is called by WorldScene, to draw over everything
        static void _Draw(sf::RenderTarget& surface);
        
        // draws the screens lightmap to the provided surface
        static void DrawEachChunksLightmaps();
        
        // @returns true everytime chunks should be updated for lighting
        static void DrawLightSources();
        static bool ShouldUpdateChunkLighting();
        static void ResetLightDelay();

        // should be called by World::Update() or any other update methods

        static void AddLightSource(LightSource* light_source);
        static void DeleteLightSource(LightSource* light_source);
        static void ClearLightSources();

        static sf::RenderTexture& GetLightTexture(){return lighting_texture;}


        static int background_shadow_offset;
        static sf::Color sunlight_colour;
        static bool show_lighting;

    private:

        // @returns true if the chunk is valid, false otherwise (inactive or out of bounds)
        static bool CanPropogateToNewChunk(sf::Vector2i chunk_coord);
        // resets all lighting values to 0 in the closed tile vector, wiping the slate clean for more propogation!
        static void ResetClosedTileVectorForChunk(sf::Vector2i chunk_coord);

        // to save performance dont update lighting every single frame!
        static float light_update_delay_tracked;
        static float previous_time_of_day_light_flush; // since daylight changes, lighting of skylight should too,
        static float light_flush_difference; // the difference required for a light flush

        static std::vector<LightSource*> light_sources;
        static std::vector<Chunk*> light_propogation_explored_chunks;

        static World* world;

        static sf::RenderTexture lighting_texture;
        static sf::RenderTexture tile_fill_texture;
        static sf::RenderTexture ambient_occlusion_texture;
        static sf::RenderTexture back_lighting_texture;
}; 