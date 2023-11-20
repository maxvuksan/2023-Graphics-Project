#include "PathfindingGraph.h"
#include "../World/World.h"


PathfindingGraph::PathfindingGraph(){
}

void PathfindingGraph::LinkWorld(World* world){
    this->world = world;
    world_profile = world->GetWorldProfile();
}


void PathfindingGraph::Update(){
    this->ConstructNodeGrid();

    
}


void PathfindingGraph::ConstructNodeGrid(){

    std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();

    if(chunks->size() <= 0){
        return;
    }

    nodes.clear();
    active_chunk_offset = (*chunks)[0][0]->GetTransform()->position;
    
    for(int chunk_y = 0; chunk_y < world_profile->height; chunk_y++){
        for(int chunk_x = 0; chunk_x < world_profile->width; chunk_x++){

            Chunk* curr_chunk = chunks->at(chunk_x).at(chunk_y);

            for(int x = 0; x < world_profile->tilemap_profile.width; x++){
            
                for(int y = 0; y < world_profile->tilemap_profile.height; y++){
                    
                    // establishing new y vector, (new row)
                    if(chunk_x == 0){
                        nodes.push_back({});
                    }
                
                                
                    Node new_node = {false};

                    // curr_chunk->IsActive()                 is the tile empty? (valid)
                    if(true && curr_chunk->GetTilemap(SetLocation::FOREGROUND)->GetTile(x, y) <= -1){
                        new_node.open = true;
                    }
                    
                    nodes.at(chunk_y * world_profile->tilemap_profile.height + y).push_back(new_node);
                }
            }

        }
    }
}



void PathfindingGraph::DrawDebug(sf::RenderTarget& surface){

    sf::VertexArray points;

    // shorthands
    float half_t_width = world_profile->tilemap_profile.tile_width / 2.0f;
    float half_t_height = world_profile->tilemap_profile.tile_height / 2.0f;

    for(int y = 0; y < nodes.size(); y++){
        for(int x = 0; x < nodes.at(y).size(); x++){

            sf::Vertex vertex;

            if(nodes[y][x].open){
                vertex.color = sf::Color::Green;
            }
            else{
                vertex.color = sf::Color::Red;
            }

            vertex.position = Camera::WorldToScreenPosition(sf::Vector2f(
                x * world_profile->tilemap_profile.tile_width + half_t_width
               ,y * world_profile->tilemap_profile.tile_height + half_t_height));
            
            points.append(vertex);
        }
    }
    points.setPrimitiveType(sf::Points);

    surface.draw(points);
}
