#include "Minimap.h"
#include "../LightSource.h"

class World;
class LightingManager : public Object{

    public:

        static void LinkWorld(World* world);

        static void PropogateLighting(sf::Vector2i coordinate, const sf::Color& colour, float decay = 0.1f);
        static void PropogateSkyLighting(sf::Vector2i coordinate, byte skylight_value);

        void Update() override;
        void Draw(sf::RenderTarget& surface) override;
        // @returns true everytime chunks should be updated for lighting
        static void DrawLightSources();
        static bool ShouldUpdateChunkLighting();
        static void ResetLightDelay();

        // should be called by World::Update() or any other update methods

        static void AddLightSource(LightSource* light_source);
        static void DeleteLightSource(LightSource* light_source);

        static sf::RenderTexture& GetLightTexture(){return lighting_texture;}


        static sf::Color sunlight_colour;
        static bool show_lighting;

    private:


        // to save performance dont update lighting every single frame!
        static float lighting_update_delay;
        static float light_update_delay_tracked;

        static std::vector<LightSource*> light_sources;

        static World* world;

        static sf::RenderTexture lighting_texture;
        static sf::RenderTexture back_lighting_texture;
};