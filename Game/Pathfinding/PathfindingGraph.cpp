#include "PathfindingGraph.h"
#include "../World/World.h"
#include <queue>


std::vector<sf::Vector2i> PathfindingGraph::previous_traversal;
WorldProfile* PathfindingGraph::world_profile;
World* PathfindingGraph::world; 

void PathfindingGraph::LinkWorld(World* _world){
    world = _world;
    world_profile = world->GetWorldProfile();

}



std::vector<sf::Vector2i> PathfindingGraph::RequestPathWorld(sf::Vector2f start_world, sf::Vector2f end_world, int max_node_coverage){
    
    int start_x = Calc::Clamp(ceil((start_world.x - ItemDictionary::half_tile_size) / (float)world_profile->tilemap_profile.tile_width ), 0, world_profile->tilemap_profile.width * world_profile->width);
    int start_y = Calc::Clamp(ceil((start_world.y - ItemDictionary::half_tile_size) / (float)world_profile->tilemap_profile.tile_height), 0, world_profile->tilemap_profile.height * world_profile->height);

    int end_x = Calc::Clamp(ceil(end_world.x / (float)world_profile->tilemap_profile.tile_width), 0, world_profile->tilemap_profile.width * world_profile->width);
    int end_y = Calc::Clamp(ceil(end_world.y / (float)world_profile->tilemap_profile.tile_height), 0, world_profile->tilemap_profile.height * world_profile->height);

    sf::Vector2i start_chunk = world->ChunkFromCoord(start_x, start_y);
    sf::Vector2i end_chunk = world->ChunkFromCoord(start_x, start_y);

    // the start chunk has no pathfinding nodes calculated
    if(world->GetChunks()->at(start_chunk.x)[start_chunk.y]->GetPathfindingGrid().size() == 0){
        return {};
    }

    // the end chunk has no pathfinding nodes calculated
    if(world->GetChunks()->at(end_chunk.x)[end_chunk.y]->GetPathfindingGrid().size() == 0){
        return {};
    }

    return RequestPath(sf::Vector2i(start_x, start_y), sf::Vector2i(end_x, end_y), TraversalMode::BREADTH_FIRST_SEARCH, max_node_coverage);
}


int PathfindingGraph::EstablishChunkPathData(std::vector<ChunkPathData>& chunk_paths, sf::Vector2i chunk_coordinate){
    
    // check if chunk has already been established
    for(int i = 0; i < chunk_paths.size(); i++){
        if(chunk_paths.at(i).chunk_coordinate == chunk_coordinate){
            return i;
        }
    }

    // other wise, establish chunk,

    chunk_paths.push_back({chunk_coordinate, sf::Vector2i(chunk_coordinate.x * world_profile->tilemap_profile.width, chunk_coordinate.y * world_profile->tilemap_profile.height), {}, {}});

    chunk_paths[chunk_paths.size() - 1].nodes.resize(world_profile->tilemap_profile.width, {});
    chunk_paths[chunk_paths.size() - 1].closed.resize(world_profile->tilemap_profile.width, {});
    chunk_paths[chunk_paths.size() - 1].parent_node_data.resize(world_profile->tilemap_profile.width, {});

    for(int x = 0; x < world_profile->tilemap_profile.width; x++){
        
        chunk_paths[chunk_paths.size() - 1].nodes.at(x).resize(world_profile->tilemap_profile.height);
        chunk_paths[chunk_paths.size() - 1].closed.at(x).resize(world_profile->tilemap_profile.height, false);
        chunk_paths[chunk_paths.size() - 1].parent_node_data.at(x).resize(world_profile->tilemap_profile.height);

        for(int y = 0; y < world_profile->tilemap_profile.height; y++){
            // reset all tiles f cost
            chunk_paths[chunk_paths.size() - 1].nodes[x][y].f_cost = 999999.0f;
            chunk_paths[chunk_paths.size() - 1].nodes[x][y].chunk_index = chunk_paths.size() - 1;
            chunk_paths[chunk_paths.size() - 1].nodes[x][y].coordinate.x = x;
            chunk_paths[chunk_paths.size() - 1].nodes[x][y].coordinate.y = y;
            chunk_paths[chunk_paths.size() - 1].parent_node_data[x][y] = {0, sf::Vector2i(0,0)};
        }
    }

    std::vector<std::vector<PathfindingNode>>& pathfinding_grid = world->GetChunks()->at(chunk_coordinate.x).at(chunk_coordinate.y)->GetPathfindingGrid();

    for(int x = 0; x < pathfinding_grid.size(); x++){
        for(int y = 0; y < pathfinding_grid.at(x).size(); y++){
            pathfinding_grid[x][y].explored = false;
        }
    }

    return chunk_paths.size() - 1;
}


std::vector<sf::Vector2i> PathfindingGraph::RequestPath(sf::Vector2i start, sf::Vector2i end, TraversalMode traversal_mode, int max_node_coverage){

    if(start == end){
        previous_traversal = {};
        return {};
    }

    std::vector<ChunkPathData> chunk_paths;


    // using A* approach

    // how the priority queue should compare the data type (NodeData)
    auto f_cost_compare = [](NodeData a, NodeData b){
        return a.f_cost >= b.f_cost;
    };

    // queue of node coordinates
    std::priority_queue<NodeData, std::vector<NodeData>, decltype(f_cost_compare)> queue(f_cost_compare);

    sf::Vector2i start_chunk_coordinate = world->ChunkFromCoord(start.x, start.y);

    EstablishChunkPathData(chunk_paths, start_chunk_coordinate);
    queue.push({0, world->OffsetFromCoord(start.x, start.y, start_chunk_coordinate.x, start_chunk_coordinate.y), 0});

    sf::Vector2i neighbours[8] = {
                                sf::Vector2i(-1, 0), 
                                sf::Vector2i(1, 0),
                                sf::Vector2i(0, 1),
                                sf::Vector2i(0, -1),
                                sf::Vector2i(-1, 1), 
                                sf::Vector2i(1, -1),
                                sf::Vector2i(1, 1),
                                sf::Vector2i(-1, -1),
                                    };
 
    // how expensive moving on that neighbour is
    float step_distance[8] = {
        1,
        1,
        1,
        1,
        1.4,
        1.4,
        1.4,
        1.4
    };

    // 10 for left right up down, 14 for diagonals 
   // std::cout << "passed setup\n";

    int steps = 0;
    while(!queue.empty()){

        steps++;
        if(steps > max_node_coverage){
            break;
        }

        NodeData node = queue.top();
        chunk_paths[node.chunk_index].closed.at(node.coordinate.x).at(node.coordinate.y) = true;
        queue.pop();

        // we found our destination
        if(chunk_paths[node.chunk_index].chunk_coordinate_tile_perspective + node.coordinate == end){
            previous_traversal = CalculatePathFromTraversal(start, node, chunk_paths);
            return previous_traversal;
        }


       // std::cout << "iterate\n";

        // pushing each new coordinate to the queue (if valid)
        for(int i = 0; i < 8; i++){

            sf::Vector2i new_coord = node.coordinate + neighbours[i];
            int new_chunk_index = node.chunk_index;

            // we are crossing over chunks
            if(new_coord.x < 0 || new_coord.x >= world_profile->tilemap_profile.width ||
            new_coord.y < 0 || new_coord.y >= world_profile->tilemap_profile.height
            ){

                sf::Vector2i new_chunk_coord = chunk_paths[node.chunk_index].chunk_coordinate;

                if(new_coord.x < 0){
                    new_chunk_coord.x--;
                    new_coord.x = world_profile->tilemap_profile.width - 1;
                }
                else if(new_coord.x >= world_profile->tilemap_profile.width) {
                    new_chunk_coord.x++;
                    new_coord.x = 0;
                }

                if(new_coord.y < 0){
                    new_chunk_coord.y--;
                    new_coord.y = world_profile->tilemap_profile.height - 1;
                }
                else if(new_coord.y >= world_profile->tilemap_profile.height) {
                    new_chunk_coord.y++;
                    new_coord.y = 0;
                }

                // new tile is out of the world bounds
                if(!world->ChunkInBounds(new_chunk_coord.x, new_chunk_coord.y)){
                    continue;
                }

                new_chunk_index = EstablishChunkPathData(chunk_paths, new_chunk_coord);
            }
            
            //std::cout << "passed chunk bound\n";


            //std::cout << new_coord.x << " " << new_coord.y << "\n";
            // the target node is open
            if(!chunk_paths[new_chunk_index].closed[new_coord.x][new_coord.y]){

              //  std::cout << "open\n";

                std::vector<std::vector<PathfindingNode>>& pathfinding_grid = world->GetChunks()->at(chunk_paths[new_chunk_index].chunk_coordinate.x).at(chunk_paths[new_chunk_index].chunk_coordinate.y)->GetPathfindingGrid();

                if(pathfinding_grid.size() == 0){
                    continue;
                }

                // only allows traversal either next to blocks or on walls, 
                if(!pathfinding_grid[new_coord.x][new_coord.y].has_block 
                && (pathfinding_grid[new_coord.x][new_coord.y].has_wall || pathfinding_grid[new_coord.x][new_coord.y].next_to_block)){


                    pathfinding_grid[new_coord.x][new_coord.y].explored = true;
                    
                    // add to previous node distance
                    chunk_paths[new_chunk_index].nodes[new_coord.x][new_coord.y].distance_from_start = 
                        step_distance[i] 
                        + chunk_paths[node.chunk_index].nodes[node.coordinate.x][node.coordinate.y].distance_from_start;

                    // calculate f cost
                    chunk_paths[new_chunk_index].nodes[new_coord.x][new_coord.y].f_cost = 
                        chunk_paths[new_chunk_index].nodes[new_coord.x][new_coord.y].distance_from_start
                        + Calc::Distance(new_coord + chunk_paths[new_chunk_index].chunk_coordinate_tile_perspective, end);
                    
                    chunk_paths[node.chunk_index].closed[node.coordinate.x][node.coordinate.y] = true;

                    chunk_paths[new_chunk_index].parent_node_data[new_coord.x][new_coord.y].chunk_index = node.chunk_index;
                    chunk_paths[new_chunk_index].parent_node_data[new_coord.x][new_coord.y].coordinate = node.coordinate;

                    queue.push({new_chunk_index, new_coord, chunk_paths[new_chunk_index].nodes[new_coord.x][new_coord.y].f_cost});
                }
            }
            


        }
    }

    previous_traversal = {};
    return {};
}


std::vector<sf::Vector2i> PathfindingGraph::CalculatePathFromTraversal(sf::Vector2i start, NodeData end_node, std::vector<ChunkPathData>& chunk_paths){
    

    // backtracking through parent_node vector, storing each node found on the path in a return vector
    std::vector<sf::Vector2i> path_so_far;
    NodeData curr = end_node;
    
    curr = chunk_paths[curr.chunk_index].parent_node_data[curr.coordinate.x][curr.coordinate.y];

    start.x %= world_profile->tilemap_profile.width;
    start.y %= world_profile->tilemap_profile.height;
    
    // loop until start reached
    while(!(curr.coordinate == start && curr.chunk_index == 0)){

        path_so_far.push_back(
            sf::Vector2i((curr.coordinate.x + chunk_paths[curr.chunk_index].chunk_coordinate_tile_perspective.x) * ItemDictionary::tile_size + ItemDictionary::half_tile_size,
            (curr.coordinate.y + chunk_paths[curr.chunk_index].chunk_coordinate_tile_perspective.y) * ItemDictionary::tile_size + ItemDictionary::half_tile_size));

        curr = chunk_paths[curr.chunk_index].parent_node_data[curr.coordinate.x][curr.coordinate.y];
    }

    std::reverse(path_so_far.begin(), path_so_far.end());


    return path_so_far;


}

void PathfindingGraph::DrawDebug(sf::RenderTarget& surface){
    
    // Draw Previous Traversal (path) -------------------------------------

    sf::VertexArray points;
    points.clear();

    for(int i = 0; i < previous_traversal.size(); i++){
        sf::Vertex vertex;
        vertex.color = sf::Color::Yellow;
        vertex.position = Camera::WorldToScreenPosition(sf::Vector2f(
                previous_traversal[i].x
               ,previous_traversal[i].y));
        points.append(vertex);
    }

    points.setPrimitiveType(sf::LineStrip);
    surface.draw(points);
}
