#include "../../Amber/Framework.h"

class World;
class Chunk;
class WaterManager : public Object {

    public:

        static void LinkWorld(World* world);
        
        void Update() override;

        static void AddNewAwakeChunk(const sf::Vector2i& chunk_coordinate);
        static void RemoveAwakeChunk(const sf::Vector2i& chunk_coordinate);
        static void AwakenSurroundingChunks(Chunk* chunk);


        void SwapTile(Chunk* chunk, int from_x, int from_y, int new_x, int new_y);

    private:

        static World* world;
        float water_update_delay_tracked;
        static float water_update_delay;
        
        // how long awake chunks stay awake while idle
        static short awake_decay;

        // what chunks should we simulate water in
        static std::vector<int> awake_chunks;

};