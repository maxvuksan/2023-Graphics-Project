#pragma once
#include "../../Amber/Framework.h"

class World;
class WorldProfile;

enum TraversalMode {
    BREADTH_FIRST_SEARCH,
    ASTAR,
};

struct PathfindingNode{
    bool open; 

    float f_cost;
};

/*
    Creates a graph representation of the world (thus relies on LinkWorld()), aiming to allow AI to navigate terrain
*/
class PathfindingGraph : public Object {

    public:

        void LinkWorld(World* world);
        
        void DrawDebug(sf::RenderTarget& surface) override;
        void Update() override;

        // converts start and end world based positions to node based positions, then @returns series of points representing the path between start and end
        static std::vector<sf::Vector2i> RequestPathWorld(sf::Vector2f start_world, sf::Vector2f end_world, int max_node_coverage = 5000);
        void ConstructNodeGrid();
        
    private:
        // @returns true if the coordinate is within the node grid bounds
        static bool CoordinateWithinBounds(const sf::Vector2i& coordinate);
        static std::vector<sf::Vector2i> RequestPath(sf::Vector2i start, sf::Vector2i end, TraversalMode traversal_mode = TraversalMode::BREADTH_FIRST_SEARCH, int max_node_coverage = 5000);
        // called by Traverse(), performs backtracking to determine the path of traversal
        static std::vector<sf::Vector2i> CalculatePathFromTraversal(sf::Vector2i start, sf::Vector2i end, std::vector<std::vector<sf::Vector2i>>& parent_node);

        static std::vector<sf::Vector2i> previous_traversal;
        static std::vector<std::vector<bool>> previous_traversal_closed; // holds if the each node is closed by the traversal function
        
        static sf::Vector2f node_offset_from_chunk; 

        static std::vector<std::vector<PathfindingNode>> nodes;

        static WorldProfile* world_profile;
        static World* world;        
};