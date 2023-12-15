#include "World.h"

class TileBehaviourManager {

    public:

        static void LinkWorld(World* world);
        static void PerformTileUpdatePass();
        static void UpdateTile(int x, int y);

    private:
        static int tickrate;
        static World* world;
};