#include "../../Amber/Framework.h"

class World;
class WorldProfile;

enum TraversalMode {
    BREADTH_FIRST_SEARCH,
    ASTAR,
};

struct PathfindingNode{
    bool open; 
};

class PathfindingGraph : public Object {

    public:

        void LinkWorld(World* world);
        
        void DrawDebug(sf::RenderTarget& surface) override;
        void Update() override;

        // converts start and end world based positions to node based positions, then @returns series of points representing the path between start and end
        std::vector<sf::Vector2i> RequestPathWorld(sf::Vector2f start_world, sf::Vector2i end_world, int max_node_coverage = 5000);
        void ConstructNodeGrid();

    private:
        
        std::vector<sf::Vector2i> RequestPath(sf::Vector2i start, sf::Vector2i end, TraversalMode traversal_mode = TraversalMode::BREADTH_FIRST_SEARCH, int max_node_coverage = 200);
        // called by Traverse(), performs backtracking to determine the path of traversal
        static std::vector<sf::Vector2i> CalculatePathFromTraversal(sf::Vector2i start, sf::Vector2i end, std::vector<std::vector<sf::Vector2i>>& parent_node);

        std::vector<sf::Vector2i> previous_traversal;

        sf::Vector2f active_chunk_offset; // the offset of nodes in the vector
        std::vector<std::vector<PathfindingNode>> nodes;

        WorldProfile* world_profile;
        World* world;        
};