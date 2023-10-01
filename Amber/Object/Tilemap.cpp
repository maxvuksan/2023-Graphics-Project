#include "Tilemap.h"
#include "Object.h"
#include "../Core/Core.h"
#include "../Core/Scene.h"
#include "PointLight.h"
#include "Camera.h"
#include <math.h>

void Tilemap::Start(){
    
    object->GetScene()->AddTilemap(this);
}
void Tilemap::OnDestroy(){
    object->GetScene()->RemoveTilemap(this);
}

void Tilemap::Update(){
    
    tilemap_primitive.setPosition(object->GetScene()->GetActiveCamera()->WorldToScreenPosition(
        object->GetTransform()->position
    ));

    tilemap_primitive.setScale(object->GetTransform()->scale);
    // rotation should have no effect on a tilemap (we cannot rotate)

    if(has_changed){
        CalculateEdges();
    }
    has_changed = false; // reset changed value
}

int Tilemap::GetTile(unsigned int x, unsigned int y){

    std::vector<std::vector<int>>& grid = tilemap_primitive.GetGrid();

    if(x < size.x && x >= 0){
        if(y < size.y && y >= 0){
            return grid[x][y];
        }
    }
    return -1;
}

bool Tilemap::TileConfigurationNeedsCorner(unsigned int x, unsigned int y){
    
    std::vector<std::vector<int>>& grid = tilemap_primitive.GetGrid();

    int C = GetTile(x, y);
    int T = GetTile(x, y - 1);
    int L = GetTile(x - 1, y);
    int TL = GetTile(x - 1, y - 1);

    if(C == -1 && TL != -1 && L == -1 && T == -1){
        return true;
    }
    if(C == -1 && TL == -1 && L != -1 && T == -1){
        return true;
    }
    if(C == -1 && TL == -1 && L == -1 && T != -1){
        return true;
    }
    if(C != -1 && TL == -1 && L == -1 && T == -1){
        return true;
    }

    return false;
}

void Tilemap::CalculateEdges(){
   
    Transform* transform = object->GetTransform();

    struct CellEdges{
        int LEFT = -1;
        int RIGHT = -1;
        int BOTTOM = -1;
        int TOP = -1;
    };

    // edge relations of each tile
    std::vector<std::vector<CellEdges>> edge_grid;
    edge_grid.resize(size.x);

    int id = 0;
    

    for(int x = 0; x < size.x; x++){
        edge_grid[x].resize(size.y);
        for(int y = 0; y < size.y; y++){

            if(GetTile(x, y) == -1){
                continue; // ignore empty edges
            }

            int L = GetTile(x - 1, y);
            int R = GetTile(x + 1, y);
            int B = GetTile(x, y + 1);
            int T = GetTile(x, y - 1);

            bool add_edge = true;    
            
            if(L == -1){ // needs left edge
                
                add_edge = true;
                if(y - 1 >= 0){
                    
                    if(edge_grid.at(x).at(y - 1).LEFT != -1){
                        add_edge = false;
                        // extend current edge

                        edge_grid[x][y].LEFT = edge_grid[x][y - 1].LEFT;
                        edges[edge_grid[x][y].LEFT].end.y += tile_size.y * transform->scale.y;
            
                    }
                }

                if(add_edge){
                    
                    Edge new_edge;
                    
                    new_edge.start = sf::Vector2f(
                        x * tile_size.x * transform->scale.x + transform->position.x,
                        y * tile_size.y * transform->scale.y + transform->position.y);

                    new_edge.end = new_edge.start;
                    new_edge.end.y += tile_size.y * transform->scale.y;
                    new_edge.id = id++;

                    edges.emplace_back(new_edge);

                    edge_grid[x][y].LEFT = new_edge.id;
                }
            }
            if(R == -1){ // needs right edge
                
                add_edge = true;
                if(y - 1 >= 0){

                    if(edge_grid.at(x).at(y - 1).RIGHT != -1){
                        add_edge = false;

                        // extend current edge
                        edge_grid[x][y].RIGHT = edge_grid[x][y - 1].RIGHT;
                        edges[edge_grid[x][y].RIGHT].end.y += tile_size.y * transform->scale.y;
                    }
                }

                if(add_edge){
                    
                    Edge new_edge;
                    
                    new_edge.start = sf::Vector2f(
                        (x + 1) * tile_size.x * transform->scale.x + transform->position.x,
                        y * tile_size.y * transform->scale.y + transform->position.y);

                    new_edge.end = new_edge.start;
                    new_edge.end.y += tile_size.y * transform->scale.y;
                    new_edge.id = id++;

                    edges.emplace_back(new_edge);

                    edge_grid[x][y].RIGHT = new_edge.id;
                }
            }
            if(T == -1){ // needs top edge
                
                add_edge = true;
                if(x - 1 >= 0){

                    if(edge_grid.at(x - 1).at(y).TOP != -1){
                        add_edge = false;

                        // extend current edge
                        edge_grid[x][y].TOP = edge_grid[x - 1][y].TOP;
                        edges[edge_grid[x][y].TOP].end.x += tile_size.x * transform->scale.x;
                    }
                }

                if(add_edge){
                    
                    Edge new_edge;
                    
                    new_edge.start = sf::Vector2f(
                        x * tile_size.x * transform->scale.x + transform->position.x,
                        y * tile_size.y * transform->scale.y + transform->position.y);

                    new_edge.end = new_edge.start;
                    new_edge.end.x += tile_size.x * transform->scale.x;
                    new_edge.id = id++;

                    edges.emplace_back(new_edge);

                    edge_grid[x][y].TOP = new_edge.id;
                }
            }

            if(B == -1){ // needs bottom edge
                
                add_edge = true;
                if(x - 1 >= 0){

                    if(edge_grid.at(x - 1).at(y).BOTTOM != -1){
                        add_edge = false;

                        // extend current edge
                        edge_grid[x][y].BOTTOM = edge_grid[x - 1][y].BOTTOM;
                        edges[edge_grid[x][y].BOTTOM].end.x += tile_size.x * transform->scale.x;
                    }
                }

                if(add_edge){
                    
                    Edge new_edge;
                    
                    new_edge.start = sf::Vector2f(
                        x * tile_size.x * transform->scale.x + transform->position.x,
                        (y + 1) * tile_size.y * transform->scale.y + transform->position.y);

                    new_edge.end = new_edge.start;
                    new_edge.end.x += tile_size.x * transform->scale.x;
                    new_edge.id = id++;

                    edges.emplace_back(new_edge);

                    edge_grid[x][y].BOTTOM = new_edge.id;
                }
            }

        }
    }
}


void Tilemap::Draw(sf::RenderTarget& surface){

    if(!this->loaded){
        return;
    }

    surface.draw(tilemap_primitive);
}

void Tilemap::Draw_Debug(sf::RenderTarget& surface){
    if(!this->loaded){
        return;
    }

    // very inefficent to draw edges individually, but because this is purley for debugging i dont mind!

    
    Camera* cam = object->GetScene()->GetActiveCamera();

    for(int i = 0; i < edges.size(); i++){

        sf::VertexArray vertex_array;
        vertex_array.setPrimitiveType(sf::Lines);

        sf::Vertex vertex;
        vertex.color = Globals::DEBUG_COLOUR;
        vertex.position = cam->WorldToScreenPosition(edges[i].end);   
        vertex_array.append(vertex);
        vertex.position = cam->WorldToScreenPosition(edges[i].start);   
        vertex_array.append(vertex);  

        vertex.color = Globals::DEBUG_COLOUR_SECONDARY;

        surface.draw(vertex_array);
        surface.draw(&vertex, 1, sf::Points);
    }
}

void Tilemap::Draw_ShadowPass(sf::RenderTarget& surface){
    
    if(!this->loaded){
        return;
    }

    std::vector<PointLight*>* point_lights = object->GetScene()->GetPointLights(); 

    Camera* camera = object->GetScene()->GetActiveCamera();

    // drawing shadow projections
    for(auto& light : *point_lights){
        // from each light source draw shadows
        
        sf::VertexArray shadows;
        shadows.setPrimitiveType(sf::Quads);

        sf::Vertex vertex;
        vertex.color = Globals::BASE_SHADOW_COLOUR;
        
        int i = 0;
        for(auto& edge : edges){


            sf::Vector2f light_pos = camera->WorldToScreenPosition(light->GetObject()->GetTransform()->position);
            sf::Vector2f start = camera->WorldToScreenPosition(edge.start);
            sf::Vector2f end = camera->WorldToScreenPosition(edge.end);

            vertex.position = start;
            shadows.append(vertex);          

            float angle = atan2(light_pos.y - start.y, light_pos.x - start.x);
            
            sf::Vector2f triangle_1_start;
            triangle_1_start = start;
            triangle_1_start.x -= cos(angle) * 9999;
            triangle_1_start.y -= sin(angle) * 9999;    
            vertex.position = triangle_1_start;  
            shadows.append(vertex);

            angle = atan2(light_pos.y - end.y, light_pos.x - end.x);
            
            vertex.position = end;      
            vertex.position.x -= cos(angle) * 9999;
            vertex.position.y -= sin(angle) * 9999;    
            shadows.append(vertex);

            vertex.position = end;      
            shadows.append(vertex);

        }
        surface.draw(shadows);

    }
}

void Tilemap::Draw_ShadowPass_PostBlur(sf::RenderTarget& surface){
    if(!this->loaded){
        return;
    }

    tilemap_primitive.SetTexture(white_texture);
    surface.draw(tilemap_primitive);
    tilemap_primitive.RevertTexture();
}

void Tilemap::Draw_EdgeLighting(sf::RenderTarget& surface){
    tilemap_primitive.SetTexture(shadow_texture);
    surface.draw(tilemap_primitive);
    tilemap_primitive.RevertTexture();
}

bool Tilemap::Load(const char* texture_label, unsigned int tile_width, unsigned int tile_height, unsigned int width, unsigned int height, const int* tiles){
    
    sf::Texture* texture = object->GetCore()->GetAssetManager()->GetTexture(texture_label);
    
    shadow_texture = object->GetCore()->GetAssetManager()->GetTexture("Amber_Shadow");
    white_texture = object->GetCore()->GetAssetManager()->GetTexture("Amber_White");
    black_texture = object->GetCore()->GetAssetManager()->GetTexture("Amber_Black");
        
    bool state = tilemap_primitive.Load(texture, sf::Vector2u(tile_width, tile_height), width, height, tiles);
    if(state == false){
        std::cout << "ERROR : Failed to call TilemapPrimitive::Load()\n";
        return false;
    }

    size = sf::Vector2f(width, height);
    tile_size = sf::Vector2f(tile_width, tile_height);

    loaded = true;
    has_changed = true;
    return true;
}

void Tilemap::SetTile(int tile_index, unsigned int x, unsigned int y){
    tilemap_primitive.SetTile(tile_index, x, y);
    has_changed = true;
}
void Tilemap::SetRow(int tile_index, unsigned int row){    
    for(unsigned int x = 0; x < tilemap_primitive.size.x; x++){
        tilemap_primitive.SetTile(tile_index, x, row);
    }
}
void Tilemap::SetColumn(int tile_index, unsigned int column){
    for(unsigned int y = 0; y < tilemap_primitive.size.y; y++){
        tilemap_primitive.SetTile(tile_index, column, y);
    }
}
void Tilemap::SetArea(int tile_index, unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max){
    for(unsigned int x = x_min; x < x_max; x++){
        for(unsigned int y = y_min; y < y_max; y++){
            tilemap_primitive.SetTile(tile_index, x, y);            
        }
    }
}
void Tilemap::SetAll(int tile_index){
    for(unsigned int y = 0; y < tilemap_primitive.size.y; y++){
        for(unsigned int x = 0; x < tilemap_primitive.size.x; x++){
            tilemap_primitive.SetTile(tile_index, x, y);
        }
    }
}