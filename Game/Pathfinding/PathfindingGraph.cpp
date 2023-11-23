#include "PathfindingGraph.h"
#include "../World/World.h"
#include <queue>


std::vector<sf::Vector2i> PathfindingGraph::previous_traversal;
std::vector<std::vector<bool>> PathfindingGraph::previous_traversal_closed;
std::vector<std::vector<PathfindingNode>> PathfindingGraph::nodes;
WorldProfile* PathfindingGraph::world_profile;
World* PathfindingGraph::world;    

void PathfindingGraph::LinkWorld(World* _world){
    world = _world;
    world_profile = world->GetWorldProfile();
}


void PathfindingGraph::Update(){
    ConstructNodeGrid();
}


std::vector<sf::Vector2i> PathfindingGraph::RequestPathWorld(sf::Vector2f start_world, sf::Vector2f end_world, int max_node_coverage){
    
    if(nodes.size() == 0){
        std::cout << "ERROR : No nodes to traverse, PathfindingGraph::ConstructNodeGrid may need to be called\n";
        return {};
    }

    int start_x = Calc::Clamp(round(start_world.x / (float)world_profile->tilemap_profile.tile_width), 0, nodes.size() - 2);
    int start_y = Calc::Clamp(round(start_world.y / (float)world_profile->tilemap_profile.tile_height), 0, nodes.at(0).size() - 2);

    int end_x = Calc::Clamp(round(end_world.x / (float)world_profile->tilemap_profile.tile_width), 0, nodes.size() - 2);
    int end_y = Calc::Clamp(round(end_world.y / (float)world_profile->tilemap_profile.tile_height), 0, nodes.at(0).size() - 2);

    return RequestPath(sf::Vector2i(start_x, start_y), sf::Vector2i(end_x, end_y), TraversalMode::BREADTH_FIRST_SEARCH, max_node_coverage);
}

bool PathfindingGraph::CoordinateWithinBounds(const sf::Vector2i& coord){
    if(coord.x > 0 && coord.x < nodes.size() - 1){
        if(coord.y > 0 && coord.y < nodes.at(0).size() - 1){
            return true;
        }
    }
    return false;
}

std::vector<sf::Vector2i> PathfindingGraph::RequestPath(sf::Vector2i start, sf::Vector2i end, TraversalMode traversal_mode, int max_node_coverage){

    if(start == end){
        previous_traversal = {};
        return {};
    }

    struct NodeData{
        sf::Vector2i coord;
        float f_cost;
    };


    std::vector<std::vector<bool>> closed; // holds if the each node is closed
    std::vector<std::vector<sf::Vector2i>> parent_node; // holds the coord of each nodes parent 

    closed.resize(nodes.size(), {});
    parent_node.resize(nodes.size(), {});

    for(int x = 0; x < nodes.size(); x++){
        
        closed.at(x).resize(nodes.at(x).size());
        parent_node.at(x).resize(nodes.at(x).size());

        for(int y = 0; y < nodes.at(x).size(); y++){
            nodes[x][y].f_cost = 999999.0f;
        }
    }
    previous_traversal_closed = closed;




    /*
    #pragma region BREADTH_FIRST_SEARCH
    std::queue<sf::Vector2i> queue;
    queue.push(start);
    int steps = 0;
    while(!queue.empty()){

        steps++;
        if(steps > max_node_coverage){
            break;
        }

        sf::Vector2i coord = queue.front();
        closed.at(coord.x).at(coord.y) = true;
        queue.pop();

        if(coord == end){
            previous_traversal_closed = closed;
            previous_traversal = CalculatePathFromTraversal(start, end, parent_node);
            return previous_traversal;
        }


        std::vector<sf::Vector2i> offsets;

        // left
        if(coord.x > 0){
            offsets.push_back(sf::Vector2i(-1,0));
            // diagonals
            if(coord.y > 0){
                offsets.push_back(sf::Vector2i(1, -1));
            }
            if(coord.y < nodes.at(coord.x).size() - 1){
                offsets.push_back(sf::Vector2i(1, 1));
            }
        }
        // right
        if(coord.x < nodes.size() - 1){
            offsets.push_back(sf::Vector2i(1,0));
            // diagonals
            if(coord.y > 0){
                offsets.push_back(sf::Vector2i(1, -1));
            }
            if(coord.y < nodes.at(coord.x).size() - 1){
                offsets.push_back(sf::Vector2i(1, 1));
            }
        }
        // top
        if(coord.y > 0){
            offsets.push_back(sf::Vector2i(0,-1));
        }
        // bottom
        if(coord.y < nodes.at(coord.x).size() - 1){
            offsets.push_back(sf::Vector2i(0,1));
        }

        // pushing each new coordinate to the queue (if valid)
        for(sf::Vector2i& offset : offsets){
            sf::Vector2i new_coord = coord + offset;
            if(!closed[new_coord.x][new_coord.y] && nodes[new_coord.x][new_coord.y].open){
                closed[new_coord.x][new_coord.y] = true;
                parent_node[new_coord.x][new_coord.y] = coord;
                queue.push(new_coord);
            }
        }
    }

    #pragma endregion
    */

    #pragma region A_STAR


    // how the priority queue should compare the data type (NodeData)
    auto f_cost_compare = [](NodeData a, NodeData b){
        return a.f_cost > b.f_cost;
    };

    // queue of node coordinates
    std::priority_queue<NodeData, std::vector<NodeData>, decltype(f_cost_compare)> queue(f_cost_compare);

    queue.push({start});
    nodes[start.x][start.y].f_cost = 0;

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
    
    // 10 for left right up down, 14 for diagonals 

    int steps = 0;
    while(!queue.empty()){

        steps++;
        if(steps > max_node_coverage){
            break;
        }

        NodeData node = queue.top();
        closed.at(node.coord.x).at(node.coord.y) = true;
        queue.pop();

        if(node.coord == end){
            previous_traversal_closed = closed;
            previous_traversal = CalculatePathFromTraversal(start, end, parent_node);
            return previous_traversal;
        }

        // pushing each new coordinate to the queue (if valid)
        for(int i = 0; i < 8; i++){

            sf::Vector2i new_coord = node.coord + neighbours[i];

            if(CoordinateWithinBounds(new_coord) && nodes[new_coord.x][new_coord.y].open){


                if(!closed[new_coord.x][new_coord.y]){


                    nodes[new_coord.x][new_coord.y].f_cost = Calc::DistanceSimple(new_coord, start) + Calc::DistanceSimple(new_coord, end);
                    closed[new_coord.x][new_coord.y] = true;
                    parent_node[new_coord.x][new_coord.y] = node.coord;
                    queue.push({new_coord, nodes[new_coord.x][new_coord.y].f_cost});
                }
            }
        }
    }

    #pragma end

    previous_traversal = {};
    return {};
}

std::vector<sf::Vector2i> PathfindingGraph::CalculatePathFromTraversal(sf::Vector2i start, sf::Vector2i end, std::vector<std::vector<sf::Vector2i>>& parent_node){
    
    // backtracking through parent_node vector, storing each node found on the path in a return vector
    std::vector<sf::Vector2i> path_so_far;

    sf::Vector2i curr = parent_node[end.x][end.y];

    // loop until start reached
    while(curr != start){
        
        path_so_far.push_back(curr);
        curr = parent_node[curr.x][curr.y];
    }

    return path_so_far;

}

void PathfindingGraph::ConstructNodeGrid(){

    // only recalculates node grid when a change has been made
    if(!world->GetWorldNeedsPathfindingRecalculating()){
        return;
    }
    // tell the world object we are performing recalculations
    world->SetWorldNeedsPathfindingRecalculating(false);

    std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();

    if(chunks->size() <= 0){
        return;
    }

    nodes.clear();
    nodes.resize(world_profile->height * world_profile->tilemap_profile.height, {});
    previous_traversal_closed.resize(nodes.size(), {});
  
    for(int chunk_x = 0; chunk_x < world_profile->width; chunk_x++){
        for(int chunk_y = 0; chunk_y < world_profile->height; chunk_y++){

            Chunk* curr_chunk = chunks->at(chunk_x).at(chunk_y);
            
            for(int y = 0; y < world_profile->tilemap_profile.height; y++){
                for(int x = 0; x < world_profile->tilemap_profile.width; x++){
                              
        
                    PathfindingNode new_node = {false};

                    // curr_chunk->IsActive()                 is the tile empty? (valid)
                    if(true && curr_chunk->GetTilemap(SetLocation::FOREGROUND)->GetTile(x, y) <= -1){
                        new_node.open = true;
                    }
                    
                    nodes.at(chunk_x * world_profile->tilemap_profile.width + x).push_back(new_node);
                    previous_traversal_closed.at(chunk_x * world_profile->tilemap_profile.width + x).push_back(false);
                }
            }

        }
    }
}



void PathfindingGraph::DrawDebug(sf::RenderTarget& surface){

    // Draw Nodes ---------------------------------------------------------

    sf::VertexArray points;

    // shorthands
    float half_t_width = world_profile->tilemap_profile.tile_width / 2.0f;
    float half_t_height = world_profile->tilemap_profile.tile_height / 2.0f;

    for(int x = 0; x < nodes.size(); x++){
        for(int y = 0; y < nodes.at(x).size(); y++){

            // ignore closed nodes
            if(!nodes[x][y].open){
                continue;
            }


            sf::Vertex vertex;
            vertex.color = sf::Color::Green;

            if(previous_traversal_closed[x][y]){
                vertex.color = sf::Color::Blue;
            }
    
            vertex.position = Camera::WorldToScreenPosition(sf::Vector2f(
                x * world_profile->tilemap_profile.tile_width + half_t_width
               ,y * world_profile->tilemap_profile.tile_height + half_t_height));
            
            points.append(vertex);
        }
    }
    points.setPrimitiveType(sf::Points);
    surface.draw(points);

    // Draw Previous Traversal (path) -------------------------------------

    points.clear();

    for(int i = 0; i < previous_traversal.size(); i++){
        sf::Vertex vertex;
        vertex.color = sf::Color::Yellow;
        vertex.position = Camera::WorldToScreenPosition(sf::Vector2f(
                previous_traversal[i].x * world_profile->tilemap_profile.tile_width + half_t_width
               ,previous_traversal[i].y * world_profile->tilemap_profile.tile_height + half_t_height));

        points.append(vertex);
    }

    points.setPrimitiveType(sf::LineStrip);
    surface.draw(points);
}
