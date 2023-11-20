#include "../../Amber/Framework.h"

class World;
class WorldProfile;

class PathfindingGraph : public Object {

    public:

        PathfindingGraph();

        void LinkWorld(World* world);



        void DrawDebug(sf::RenderTarget& surface) override;

        void Update() override;

        void ConstructNodeGrid();

    private:

        struct Node{
            bool open; 
        };

        sf::Vector2f active_chunk_offset; // the offset of nodes in the vector
        std::vector<std::vector<Node>> nodes;


        WorldProfile* world_profile;
        World* world;        
};