#include "Chunk.h"
#include "LightingManager.h"
#include "World.h"
#include "../Utility/UtilityStation.h"

void Chunk::Init(World* world){

    this->world = world;

    SetRenderLayer(10);

    main_tilemap = this->AddComponent<Tilemap>(0);
    foreground_tilemap = this->AddComponent<Tilemap>(0);
    background_tilemap = this->AddComponent<Tilemap>(-1);

    foreground_tilemap->Load("foreground_tiles", &world->world_profile.tilemap_profile);
    main_tilemap->Load("tiles", &world->world_profile.tilemap_profile);
    background_tilemap->Load("background_tiles", &world->world_profile.tilemap_profile);
    
    // assigning a collider to the main tilemap
    auto collider = this->AddComponent<TilemapCollider>();
    collider->SetTilemap(main_tilemap);

    foliage_vertex_array.setPrimitiveType(sf::PrimitiveType::Quads);

    light_map_dirty = false;

    skylight_first_calculated = false;
}

void Chunk::Draw(sf::RenderTarget& surface){

    if(light_map_dirty){
        light_map_dirty = false;    
        light_texture.loadFromImage(light_map);
    }

    if(foliage_map_dirty){
        RecalculateFoliageVertexArray();
    }
    sf::RenderStates render_states;
    render_states.transform.translate(Scene::GetActiveCamera()->WorldToScreenPosition(sf::Vector2f(0,0)));
    render_states.texture = AssetManager::GetTexture("foliage");
    surface.draw(foliage_vertex_array, render_states);

}

void Chunk::StoreUtilityStationReference(UtilityStation* station){
    utility_stations.push_back(station);
}

void Chunk::BreakUtilityStation(UtilityStation* reference){
    
    for(int i = 0; i < utility_stations.size(); i++){
        if(utility_stations[i] == reference){
            utility_stations.erase(utility_stations.begin() + i);

            world->CreatePickup(reference->GetUtilityData()->block_data.pickup, reference->GetTransform()->position.x, reference->GetTransform()->position.y);

            RemoveObjectFromChunk(reference);

            return;
        }
    }
}



void Chunk::RemoveTorchPosition(int coord_x, int coord_y){
    for(int i = 0; i < torch_positions.size(); i++){
        if(torch_positions[i] == sf::Vector2i(coord_x, coord_y)){
            torch_positions.erase(torch_positions.begin() + i);
        }
    }
}

void Chunk::AddTorchPosition(int  coord_x, int coord_y){
    torch_positions.push_back(sf::Vector2i(coord_x, coord_y));

    //Object* particle_object = AddObjectToChunk<Object>(4);
    //particle_object->AddComponent<PixelParticleSource>()->SetOffset(sf::Vector2f(4,2));
    //particle_object->GetTransform()->position = world->CoordToWorld(coord_x, coord_y) + GetTransform()->position;

    //objects_bound_to_chunk.push_back(particle_object);
}

Tilemap* Chunk::GetTilemap(SetLocation set_location){
    if(set_location == SetLocation::MAIN){
        return main_tilemap;
    }
    else if(set_location == SetLocation::BACKGROUND){
        return background_tilemap;
    }
    else{
        return foreground_tilemap;
    }
}

signed_byte Chunk::GetTile(int x, int y, SetLocation get_location){
    
    if(get_location == SetLocation::MAIN){
        return main_tilemap->GetTile(x, y);
    }
    else if(get_location == SetLocation::BACKGROUND){
        return background_tilemap->GetTile(x, y);
    }
    else{
        return foreground_tilemap->GetTile(x, y);
    }
}
EntireTile Chunk::GetEntireTile(int x, int y){
    return {main_tilemap->GetTile(x, y), 
            background_tilemap->GetTile(x, y),
            foreground_tilemap->GetTile(x, y)};
}

void Chunk::AddFoliage(Foliage foliage_index, int x, int y){
    foliage_map.insert(std::make_pair(x + y * world->tilemap_profile->width, foliage_index));
    foliage_map_dirty = true;
}

void Chunk::RemoveFoliage(int x, int y){

    int map_key = x + y * world->tilemap_profile->width;

    // does foliage exist at location?
    if(foliage_map.find(map_key) == foliage_map.end()){
        return;
    }

    // create pickup if needed
    //      world position of offset + chunk position
    sf::Vector2f world_pos = world->CoordToWorld(x, y) + GetTransform()->position;
    world->CreatePickup(ItemDictionary::FOLIAGE_DATA[foliage_map.at(map_key)].pickup, world_pos.x, world_pos.y);

    // erase 
    foliage_map.erase(x + y * world->tilemap_profile->width);
    foliage_map_dirty = true;

}

void Chunk::RecalculateFoliageVertexArray(){

    foliage_vertex_array.clear();
    
    sf::Vertex vertex;

    for(auto foliage_object : foliage_map){

        int x_coord = foliage_object.first % world->tilemap_profile->width;
        int x = x_coord * (float)world->tilemap_profile->tile_width - ItemDictionary::FOLIAGE_DATA[foliage_object.second].origin.x + 4;
        int y = (floor(foliage_object.first - x_coord) / world->tilemap_profile->width) * (float)world->tilemap_profile->tile_height - ItemDictionary::FOLIAGE_DATA[foliage_object.second].origin.y;

        // set position to chunks position + tile offset + origin

        // top left
        vertex.position = GetTransform()->position + sf::Vector2f(x, y);

        vertex.texCoords.x = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.x;
        vertex.texCoords.y = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.y;
        foliage_vertex_array.append(vertex);

        // top right
        vertex.position = GetTransform()->position + sf::Vector2f(x + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.x, y);

        vertex.texCoords.x = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.x + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.x;
        vertex.texCoords.y = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.y;
        foliage_vertex_array.append(vertex);

        // bottom right
        vertex.position = GetTransform()->position + sf::Vector2f(x + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.x, y + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.y);

        vertex.texCoords.x = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.x + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.x;
        vertex.texCoords.y = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.y + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.y;
        foliage_vertex_array.append(vertex);

        // bottom left
        vertex.position = GetTransform()->position + sf::Vector2f(x, y  + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.y);

        vertex.texCoords.x = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.x;
        vertex.texCoords.y = ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_position_on_sprite_sheet.y + ItemDictionary::FOLIAGE_DATA[foliage_object.second].pixel_dimensions.y;
        foliage_vertex_array.append(vertex);
    }

    foliage_map_dirty = false;
}


void Chunk::SetTile(signed_byte tile_index, int x, int y, SetLocation set_location){

    if(set_location == SetLocation::MAIN){
        main_tilemap->SetTileSafe(tile_index, x, y);
    }
    else if(set_location == SetLocation::BACKGROUND){
        background_tilemap->SetTileSafe(tile_index, x, y);
    }
    else{
        foreground_tilemap->SetTileSafe(tile_index, x, y);
    }
}

void Chunk::ClearLightmap(){

    light_map.create(world->world_profile.tilemap_profile.width, world->world_profile.tilemap_profile.height, sf::Color(0, 0, 0));

}

void Chunk::CalculateSkyLight(){
    
    sky_tiles.clear();
    sky_tiles_to_propogate.clear();

    skylight_first_calculated = true;
    
    for(unsigned int x = 0; x < world->world_profile.tilemap_profile.width; x++){
        for(unsigned int y = 0; y < world->world_profile.tilemap_profile.height; y++){
            
            // copied from IntroduceTileToSkylight(), slightly changed for perfomance

            // air
            if(background_tilemap->GetTile(x, y) == -1 && main_tilemap->GetTile(x,y) == -1){
                sky_tiles.push_back(sf::Vector2i(x,y));
            }
            else{
                continue;
            }

            for(auto offset : Globals::HORIZONTAL_OFFSETS){
                sf::Vector2i coord = sf::Vector2i(x,y) + world->WorldToCoord(GetTransform()->position.x, GetTransform()->position.y) + offset;

                // the surrounding tile is not a sky tile
                if(world->GetTile(coord.x, coord.y, SetLocation::MAIN) != -1 || world->GetTile(coord.x, coord.y, SetLocation::BACKGROUND) != -1){
                    
                    // prevent duplicates
                    if(std::find(sky_tiles_to_propogate.begin(), sky_tiles_to_propogate.end(), sf::Vector2i(coord.x,coord.y)) == sky_tiles_to_propogate.end()){
                        sky_tiles_to_propogate.push_back(sf::Vector2i(coord.x,coord.y));
                    }
                }
            }
        }
    }    
}
/*
void Chunk::IntroduceTileToSkylight(int x, int y){

    if(background_tilemap->GetTile(x, y) == -1 && main_tilemap->GetTile(x,y) == -1){
        // prevent duplicates
        if(std::find(sky_tiles.begin(), sky_tiles.end(), sf::Vector2i(x,y)) == sky_tiles.end()){
            sky_tiles.push_back(sf::Vector2i(x,y));
        }
    }
    else{
        return;
    }

    for(auto offset : Globals::HORIZONTAL_OFFSETS){
        sf::Vector2i coord = sf::Vector2i(x,y) + world->WorldToCoord(GetTransform()->position.x, GetTransform()->position.y) + offset;

        // the surrounding tile is not a sky tile
        if(world->GetTile(coord.x, coord.y, SetLocation::MAIN) != -1 || world->GetTile(coord.x, coord.y, SetLocation::BACKGROUND) != -1){
            
            // prevent duplicates
            if(std::find(sky_tiles_to_propogate.begin(), sky_tiles_to_propogate.end(), sf::Vector2i(coord.x,coord.y)) == sky_tiles_to_propogate.end()){
                sky_tiles_to_propogate.push_back(sf::Vector2i(coord.x,coord.y));
            }

        }
    }
}

void Chunk::RemoveTileFromSkylight(int x, int y){

    sf::Vector2i chunk_coord = world->WorldToCoord(GetTransform()->position.x, GetTransform()->position.y);
    sf::Vector2i coord = sf::Vector2i(x, y) + chunk_coord;
    
    bool found_skylight = false;  
    bool all_skylight = true;  

    for(auto offset : Globals::HORIZONTAL_OFFSETS){
        sf::Vector2i shifted_coord = coord + offset;

        // the surrounding tile is a sky tile
        if(world->GetTile(shifted_coord.x, shifted_coord.y, SetLocation::MAIN) == -1 && world->GetTile(shifted_coord.x, shifted_coord.y, SetLocation::BACKGROUND) == -1){
            found_skylight = true;
        }
        else{
            all_skylight = false;
        }
    }

    // surrounded all by skylight
    if(all_skylight){
        auto location = std::find(sky_tiles.begin(), sky_tiles.end(), sf::Vector2i(x,y));
        
        if(location != sky_tiles.end()){
            sky_tiles.erase(location);
        }
    }
    // no other offsets have
    else if(!found_skylight){

        auto location = std::find(sky_tiles_to_propogate.begin(), sky_tiles_to_propogate.end(), sf::Vector2i(coord.x,coord.y));
        
        if(location != sky_tiles_to_propogate.end()){
            sky_tiles_to_propogate.erase(location);
        }
    }
}
*/
void Chunk::PropogateTorches(){
    

    // sky light
    for(int i = 0; i < sky_tiles.size(); i++){
        light_map.setPixel(sky_tiles.at(i).x, sky_tiles.at(i).y, LightingManager::sunlight_colour);
    }
    MarkLightmapDirty();

    for(int i = 0; i < sky_tiles_to_propogate.size(); i++){
        LightingManager::PropogateLighting(sky_tiles_to_propogate.at(i), LightingManager::sunlight_colour, 0.06f);    
    }

    sf::Vector2i coord = world->WorldToCoord(GetTransform()->position.x, GetTransform()->position.y);

    // placed light
    for(int i = 0; i < torch_positions.size(); i++){
        LightingManager::PropogateLighting(coord + torch_positions[i], ItemDictionary::torch_colour, ItemDictionary::torch_propogate_decay);
    }

}

void Chunk::RefreshSkylight(){

    if(!skylight_first_calculated){
        CalculateSkyLight();
    }
}

void Chunk::OnSetActive(){
    
    
    lighting_closed_grid.resize(world->tilemap_profile->width);
    for(int i = 0; i < lighting_closed_grid.size(); i++){
        lighting_closed_grid[i].resize(world->tilemap_profile->height, 0);
    }

    RecalculateFoliageVertexArray();
    if(!skylight_first_calculated){
        CalculateSkyLight();
    }
    ClearLightmap();
    
    light_map_dirty = true;

    main_tilemap->ConstructVertexArray();
    foreground_tilemap->ConstructVertexArray();
    background_tilemap->ConstructVertexArray();
}

void Chunk::OnDisable(){

    lighting_closed_grid.clear();
    foliage_vertex_array.clear();

    light_map.create(1,1);
    light_texture.create(1,1);
    main_tilemap->ClearVertexArray();
    foreground_tilemap->ClearVertexArray();
    background_tilemap->ClearVertexArray();
}

void Chunk::ClearColliders(){
    GetComponent<TilemapCollider>()->ClearRects();
}
void Chunk::ResetCollidersIfChanged(){
    if(dirty){
        GetComponent<TilemapCollider>()->Reset();
        dirty = false;
    }
}
void Chunk::ResetColliders(){
    GetComponent<TilemapCollider>()->Reset();
}

Chunk::~Chunk(){
    for(auto obj : objects_bound_to_chunk){
        Memory::Delete<Object>(obj, __FUNCTION__);
    }
    for(auto obj : ui_bound_to_chunk){
        Memory::Delete<Object>(obj, __FUNCTION__);
    }
}