#include "../Minimap.h"
#include "LightSource.h"

class World;
class Chunk;
class LightingManager : public Object{

    friend class BackgroundShadowManager;

    public:

        static void LinkWorld(World* world);

        /*
            propogates a light source on the grid of the world 

            @param coordinate of the light source
            @param colour to tint the light
            @param decay how fast the light drops off
            @param decay_dropoff is multiplied by the decay each step
        */
        static void PropogateLighting(sf::Vector2i coordinate, const sf::Color& colour, float decay = 0.1f);

        void Update() override;

        void Draw(sf::RenderTarget& surface) override;
        
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

        static std::vector<LightSource*> light_sources;
        static std::vector<Chunk*> light_propogation_explored_chunks;

        static World* world;

        static sf::RenderTexture lighting_texture;
        static sf::RenderTexture tile_fill_texture;
        static sf::RenderTexture ambient_occlusion_texture;
        static sf::RenderTexture back_lighting_texture;
}; 