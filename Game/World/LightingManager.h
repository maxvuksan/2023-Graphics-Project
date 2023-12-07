#include "Minimap.h"
#include "../Player/LightSource.h"

class World;
class LightingManager {

    public:

        static void LinkWorld(World* world);
        static void PropogateLighting(sf::Vector2i coordinate, float light_value = 1);

        // should be called by World::Update() or any other update methods
        static void Update();

        static void AddLightSource(LightSource* light_source);
        static void DeleteLightSource(LightSource* light_source);

    private:

        static std::vector<LightSource*> light_sources;

        static World* world;
};