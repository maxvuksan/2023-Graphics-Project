#include "LightingManager.h"
#include "../World.h"
#include "LightSource.h"
#include <queue>
#include "../../Settings.h"

World* LightingManager::world;
std::vector<LightSource*> LightingManager::light_sources;




int LightingManager::background_shadow_offset = 2;
bool LightingManager::show_lighting = true;

sf::Color LightingManager::sunlight_colour(31,30,61);


float LightingManager::light_update_delay_tracked = Settings::LIGHT_REFRESH_RATE;
float LightingManager::previous_time_of_day_light_flush = 0;
float LightingManager::light_flush_difference = 30;

sf::RenderTexture LightingManager::lighting_texture;
sf::RenderTexture LightingManager::back_lighting_texture;
sf::RenderTexture LightingManager::ambient_occlusion_texture;

std::vector<Chunk*> LightingManager::light_propogation_explored_chunks;

void LightingManager::LinkWorld(World* _world){

    world = _world;
    lighting_texture.create(Core::GetDisplayWidth() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2, Core::GetDisplayHeight() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2);
    back_lighting_texture.create(Core::GetDisplayWidth() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2, Core::GetDisplayHeight() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2);
    ambient_occlusion_texture.create(Core::GetDisplayWidth() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2, Core::GetDisplayHeight() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2);
}


void LightingManager::Update(){

    light_update_delay_tracked += Time::Dt();
}

void LightingManager::_Draw(sf::RenderTarget& surface){

    DrawEachChunksLightmaps();

    Scene::GetActiveCamera()->background_colour = sunlight_colour;

    if(show_lighting){
        std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();
 
        // blur
        AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_strength", Settings::_ACTIVE_LIGHT_BLUR_FACTOR);
        AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_strength", Settings::_ACTIVE_LIGHT_BLUR_FACTOR);

        // blurring! ----------------------
        back_lighting_texture.draw(sf::Sprite(lighting_texture.getTexture()), AssetManager::GetShader("Amber_BlurVertical"));
        back_lighting_texture.display();
        lighting_texture.draw(sf::Sprite(back_lighting_texture.getTexture()), AssetManager::GetShader("Amber_BlurHorizontal"));
        lighting_texture.display();

        surface.draw(sf::Sprite(lighting_texture.getTexture()));
    }

}

void LightingManager::DrawEachChunksLightmaps(){

    sf::Sprite light_sprite;
    light_sprite.setScale(sf::Vector2f(world->GetWorldProfile()->tilemap_profile.tile_width, world->GetWorldProfile()->tilemap_profile.tile_height));

    std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();

    // draw final lightmap
    for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
        for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){
            
            light_sprite.setColor(sf::Color::White);
            if(!chunks->at(x).at(y)->IsActive()){
                continue;
            }
            // drawing light texture (from light sources)
            light_sprite.setTexture(chunks->at(x).at(y)->GetLightmapTexture());
            light_sprite.setPosition(Camera::WorldToScreenPosition(chunks->at(x).at(y)->GetTransform()->position));    
            lighting_texture.draw(light_sprite);
        }
    }

}

bool LightingManager::ShouldUpdateChunkLighting(){ 
    if(light_update_delay_tracked > Settings::LIGHT_REFRESH_RATE){
        return true;
    }
    return false;
}

void LightingManager::DrawLightSources(){

    if(light_update_delay_tracked > Settings::LIGHT_REFRESH_RATE){


        // light flushes updating all chunks
        if(abs(previous_time_of_day_light_flush - TimeManager::GetTimeOfDay()) > light_flush_difference){
            
            previous_time_of_day_light_flush = TimeManager::GetTimeOfDay();

            // clear lightmap
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){

                    world->GetChunks()->at(x).at(y)->ClearLightmap();
                }
            }
            // draw all static light sources
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){
                    
                    world->GetChunks()->at(x).at(y)->PropogateTorches();
                    world->GetChunks()->at(x).at(y)->SetLightmapDirty(false);
                }
            }

            std::cout << "[LIGHT FLUSH]\n";
        }
        else{ // otherwise, only update dirty chunks

            // clear lightmap
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){

                    if(world->GetChunks()->at(x).at(y)->GetLightmapDirty()){
                        std::cout << "dirty " << x << " " << y << "\n";
                        world->GetChunks()->at(x).at(y)->ClearLightmap();
                    }
                }
            }
            // draw all static light sources
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){
                    
                    if(world->GetChunks()->at(x).at(y)->GetLightmapDirty()){
                        world->GetChunks()->at(x).at(y)->PropogateTorches();
                        world->GetChunks()->at(x).at(y)->SetLightmapDirty(false);
                    }
                }
            }
        }


    /*
        // draw dynamic (unbounded light sources)
        sf::Vector2f pos;
        for(int i = 0; i < light_sources.size(); i++){
            pos = light_sources[i]->GetThisObject()->GetTransform()->position + light_sources[i]->offset;
            PropogateLighting(world->WorldToCoord(pos.x, pos.y), light_sources[i]->colour, light_sources[i]->decay);
        }
    */
    }
}

void LightingManager::ResetLightDelay(){
    if(light_update_delay_tracked > Settings::LIGHT_REFRESH_RATE){
        light_update_delay_tracked = 0;
    }
}


bool LightingManager::CanPropogateToNewChunk(sf::Vector2i chunk_pos){
    if(!world->ChunkInBounds(chunk_pos.x, chunk_pos.y)){
        return false;
    }
    if(!world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y)->IsActive()){
        return false;
    }

    world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y)->MarkLightmapForTextureReload();

    return true;
}

void LightingManager::ResetClosedTileVectorForChunk(sf::Vector2i chunk_pos){
    // the chunk has not yet been touched
    if(std::find(light_propogation_explored_chunks.begin(), light_propogation_explored_chunks.end(), world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y)) == light_propogation_explored_chunks.end()){
        
        light_propogation_explored_chunks.push_back(world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y));
        
        for(int x = 0; x < world->GetWorldProfile()->tilemap_profile.width; x++){
            world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y)->lighting_closed_grid.at(x).resize(world->GetWorldProfile()->tilemap_profile.height, 0);
        }
    }
}


void LightingManager::PropogateLighting(sf::Vector2i coordinate, const sf::Color& _colour, int decay_step, int decay_dropoff, int decay_min){

    if(!LightingManager::show_lighting){
        return;
    }

    struct LightTile {
        sf::Vector2i coord;
        int r;
        int g;
        int b;
        int decay;
        sf::Vector2i chunk_origin;
    };
    
    std::queue<LightTile> queue;
    std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();
    sf::Vector2i chunk_coord = world->ChunkFromCoord(coordinate.x, coordinate.y);
    coordinate = world->OffsetFromCoord(coordinate.x, coordinate.y, chunk_coord.x, chunk_coord.y);

    light_propogation_explored_chunks.clear();

    if(!CanPropogateToNewChunk(chunk_coord)){
        return;
    }
    //ResetClosedTileVectorForChunk(chunk_coord);
    
    queue.push({coordinate, _colour.r, _colour.g, _colour.b, decay_step, sf::Vector2i(chunk_coord.x, chunk_coord.y)});

    if(!world->ChunkInBounds(chunk_coord.x, chunk_coord.y)){
        return;
    }

    Chunk* chunk;
    signed_byte current_tile;
    sf::Color old_colour;
    sf::Color new_colour;

    LightTile light_tile;

    while(!queue.empty()){

        light_tile = queue.front();    
        queue.pop();

        if(light_tile.r < 5 && light_tile.g < 5 && light_tile.b < 5){
            continue;
        }

        if(light_tile.coord.x == world->GetWorldProfile()->tilemap_profile.width){
            light_tile.coord.x = 0;
            light_tile.chunk_origin.x++;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
        }
        else if(light_tile.coord.x == -1){
            light_tile.coord.x = world->GetWorldProfile()->tilemap_profile.width - 1;
            light_tile.chunk_origin.x--;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
        }

        if(light_tile.coord.y == world->GetWorldProfile()->tilemap_profile.height){
            light_tile.coord.y = 0;
            light_tile.chunk_origin.y++;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
        }
        else if(light_tile.coord.y == -1){
            light_tile.coord.y = world->GetWorldProfile()->tilemap_profile.height - 1;
            light_tile.chunk_origin.y--;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
        }


        chunk = chunks->at(light_tile.chunk_origin.x).at(light_tile.chunk_origin.y);        

        sf::Image& light_map = chunk->GetLightmap();
        
        old_colour = light_map.getPixel(light_tile.coord.x, light_tile.coord.y);
        new_colour = sf::Color(std::max((int)old_colour.r, light_tile.r), std::max((int)old_colour.g, light_tile.g),std::max((int)old_colour.b, light_tile.b));

    
        if(old_colour.r >= new_colour.r && old_colour.g >= new_colour.g && old_colour.b >= new_colour.b){
            continue;
        }
    

        //chunk->lighting_closed_grid[light_tile.coord.x][light_tile.coord.y] = light_tile.intensity;

        light_map.setPixel(light_tile.coord.x, light_tile.coord.y, new_colour);

        current_tile = chunk->GetTilemapMain()->GetTile(light_tile.coord.x, light_tile.coord.y);

        if(current_tile != -1 && ItemDictionary::MAIN_BLOCK_DATA[current_tile].blocks_light){
            light_tile.r *= 0.3f;
            light_tile.g *= 0.3f;
            light_tile.b *= 0.3f;
        }

        int new_r = light_tile.r - 17;
        int new_g = light_tile.g - 17;
        int new_b = light_tile.b - 17;

        light_tile.decay -= decay_dropoff;
        light_tile.decay = Calc::ClampLower(light_tile.decay, decay_min);
    

        queue.push({light_tile.coord + sf::Vector2i(0, 1), new_r, new_g, new_b, light_tile.decay, light_tile.chunk_origin});
        queue.push({light_tile.coord + sf::Vector2i(0, -1), new_r, new_g, new_b, light_tile.decay, light_tile.chunk_origin});
        queue.push({light_tile.coord + sf::Vector2i(-1,0), new_r, new_g, new_b, light_tile.decay, light_tile.chunk_origin});
        queue.push({light_tile.coord + sf::Vector2i(1,0), new_r, new_g, new_b, light_tile.decay, light_tile.chunk_origin});

        //chunk->lighting_closed_grid[light_tile.coord.x][light_tile.coord.y] = light_tile.intensity;
    }

}

void LightingManager::AddLightSource(LightSource* light_source){
    light_sources.push_back(light_source);
}

void LightingManager::DeleteLightSource(LightSource* light_source){
    for(int i = 0; i < light_sources.size(); i++){
        if(light_sources[i] == light_source){
            light_sources.erase(light_sources.begin() + i);
        }
    }
}

void LightingManager::ClearLightSources(){
    light_sources.clear();
}