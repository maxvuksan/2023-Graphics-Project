#pragma once
#include "../../Amber/Framework.h"

class World;
class WorldProfile;


enum TraversalMode {
    BREADTH_FIRST_SEARCH,
    ASTAR,
};

struct PathfindingNode{

    // defines what surrounds the node
    bool has_block;
    bool has_wall; 
    bool next_to_block;
    bool explored = false;
};

/*
    Creates a graph representation of the world (thus relies on LinkWorld()), aiming to allow AI to navigate terrain
*/
class PathfindingGraph : public Object {

    public:

        void LinkWorld(World* world);
        
        void DrawDebug(sf::RenderTarget& surface) override;

        // converts start and end world based positions to node based positions, then @returns series of points representing the path between start and end
        static std::vector<sf::Vector2i> RequestPathWorld(sf::Vector2f start_world, sf::Vector2f end_world, int max_node_coverage = 8000);
        

    private:

        struct NodeData{
            int chunk_index; // the related chunk in the chunkpathdata array
            sf::Vector2i coordinate; // offset from the chunk
            float f_cost;
            int distance_from_start;
        };

        struct ChunkPathData{
            sf::Vector2i chunk_coordinate;
            sf::Vector2i chunk_coordinate_tile_perspective; // chunk coordinate * tilemap_width
            std::vector<std::vector<NodeData>> nodes;
            std::vector<std::vector<bool>> closed; // holds if each node is closed
            std::vector<std::vector<NodeData>> parent_node_data; // the node traversed to get to this node
        };

        static std::vector<sf::Vector2i> RequestPath(sf::Vector2i start, sf::Vector2i end, TraversalMode traversal_mode = TraversalMode::BREADTH_FIRST_SEARCH, int max_node_coverage = 8000);
        // @returns the chunk index of the establishded chunk
        static int EstablishChunkPathData(std::vector<ChunkPathData>& chunk_path_data, sf::Vector2i chunk_coordinate);
        // called by Traverse(), performs backtracking to determine the path of traversal
        static std::vector<sf::Vector2i> CalculatePathFromTraversal(sf::Vector2i start, NodeData end_node, std::vector<ChunkPathData>& chunk_paths);

        static std::vector<sf::Vector2i> previous_traversal;

        static WorldProfile* world_profile;
        static World* world;        
};