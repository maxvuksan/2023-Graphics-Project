#include "LightingManager.h"
#include "World.h"
#include "../LightSource.h"
#include <queue>
#include "../Settings.h"

World* LightingManager::world;
std::vector<LightSource*> LightingManager::light_sources;


bool LightingManager::show_lighting = true;

sf::Color LightingManager::sunlight_colour(31,30,61);


float LightingManager::light_update_delay_tracked = Settings::LIGHT_REFRESH_RATE;
sf::RenderTexture LightingManager::lighting_texture;
sf::RenderTexture LightingManager::back_lighting_texture;
sf::RenderTexture LightingManager::ambient_occlusion_texture;

std::vector<Chunk*> LightingManager::light_propogation_explored_chunks;

void LightingManager::LinkWorld(World* _world){

    world = _world;
    lighting_texture.create(Core::GetDisplayWidth(), Core::GetDisplayHeight());
    back_lighting_texture.create(Core::GetDisplayWidth(), Core::GetDisplayHeight());
    ambient_occlusion_texture.create(Core::GetDisplayWidth(), Core::GetDisplayHeight());
}


void LightingManager::Update(){

    light_update_delay_tracked += Time::Dt();
}

void LightingManager::Draw(sf::RenderTarget& surface){

    Scene::GetActiveCamera()->background_colour = sunlight_colour;

    if(show_lighting){
        std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();

        // blur

        AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_strength", Settings::LIGHT_BLUR_FACTOR);
        AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_strength", Settings::LIGHT_BLUR_FACTOR);

        // blurring! ----------------------
        back_lighting_texture.draw(sf::Sprite(lighting_texture.getTexture()), AssetManager::GetShader("Amber_BlurVertical"));
        back_lighting_texture.display();
        lighting_texture.draw(sf::Sprite(back_lighting_texture.getTexture()), AssetManager::GetShader("Amber_BlurHorizontal"));
        lighting_texture.display();
        //back_lighting_texture.draw(sf::Sprite(lighting_texture.getTexture()), AssetManager::GetShader("Amber_Banding"));
        //back_lighting_texture.display();

        surface.draw(sf::Sprite(lighting_texture.getTexture()), sf::BlendMultiply);

        if(Settings::AMBIENT_OCCLUSION){
            ambient_occlusion_texture.clear(sf::Color::White);

            AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_strength", Settings::AMBIENT_OCCLUSION_BLUR_FACTOR);
            AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_strength", Settings::AMBIENT_OCCLUSION_BLUR_FACTOR);

            // draw black tile siloutte
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){
                    chunks->at(x).at(y)->GetTilemap(SetLocation::MAIN)->DrawSiloutte(ambient_occlusion_texture, AssetManager::GetTexture("Amber_Shadow"));
                }
            }

            
            // blur siloutte
            ambient_occlusion_texture.display();
            back_lighting_texture.draw(sf::Sprite(ambient_occlusion_texture.getTexture()), AssetManager::GetShader("Amber_BlurVertical"));
            back_lighting_texture.display();
            ambient_occlusion_texture.draw(sf::Sprite(back_lighting_texture.getTexture()), AssetManager::GetShader("Amber_BlurHorizontal"));
            
            // cutout original tile shape over blurred
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){
                    chunks->at(x).at(y)->GetTilemap(SetLocation::MAIN)->DrawSiloutte(ambient_occlusion_texture, AssetManager::GetTexture("Amber_White"));
                }
            }


            back_lighting_texture.clear(sf::Color::Transparent);
            // mask occlusion to background walls only
            for(int x = world->GetActiveChunksMin().x; x < world->GetActiveChunksMax().x; x++){
                for(int y = world->GetActiveChunksMin().y; y < world->GetActiveChunksMax().y; y++){
                    chunks->at(x).at(y)->GetTilemap(SetLocation::BACKGROUND)->DrawSiloutte(back_lighting_texture, AssetManager::GetTexture("Amber_White"));
                }
            }   
            back_lighting_texture.display();
            ambient_occlusion_texture.draw(sf::Sprite(back_lighting_texture.getTexture()), sf::BlendMultiply);
            ambient_occlusion_texture.display();

            back_lighting_texture.clear(sf::Color::White);
            back_lighting_texture.draw(sf::Sprite(ambient_occlusion_texture.getTexture()), sf::BlendAlpha);
            back_lighting_texture.display();

            surface.draw(sf::Sprite(back_lighting_texture.getTexture()), sf::BlendMultiply);
        } 

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
            }
        }

        // draw dynamic (unbounded light sources)
        sf::Vector2f pos;
        for(int i = 0; i < light_sources.size(); i++){
            pos = light_sources[i]->GetThisObject()->GetTransform()->position + light_sources[i]->offset;
            PropogateLighting(world->WorldToCoord(pos.x, pos.y), light_sources[i]->colour, light_sources[i]->decay);
        }

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
    return true;
}

void LightingManager::ResetClosedTileVectorForChunk(sf::Vector2i chunk_pos){
    if(std::find(light_propogation_explored_chunks.begin(), light_propogation_explored_chunks.end(), world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y)) == light_propogation_explored_chunks.end()){
        light_propogation_explored_chunks.push_back(world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y));
        
        for(int x = 0; x < world->GetWorldProfile()->tilemap_profile.width; x++){
            world->GetChunks()->at(chunk_pos.x).at(chunk_pos.y)->lighting_closed_grid.at(x).resize(world->GetWorldProfile()->tilemap_profile.height, 0);
        }
    }
}


void LightingManager::PropogateLighting(sf::Vector2i coordinate, const sf::Color& _colour, float decay){

    //Time::StartRecord();

    struct LightTile {
        sf::Vector2i coord;
        float intensity; 
        sf::Vector2i chunk_origin;
    };
    
    std::queue<LightTile> queue;
    std::vector<std::vector<Chunk*>>* chunks = world->GetChunks();
    sf::Vector2i chunk_coord = world->ChunkFromCoord(coordinate.x, coordinate.y);
    coordinate = world->OffsetFromCoord(coordinate.x, coordinate.y, chunk_coord.x, chunk_coord.y);

    LightingManager::light_propogation_explored_chunks.clear();

    if(!CanPropogateToNewChunk(chunk_coord)){
        return;
    }
    ResetClosedTileVectorForChunk(chunk_coord);
    
    queue.push({coordinate, 1.0f, sf::Vector2i(chunk_coord.x, chunk_coord.y)});

    if(!world->ChunkInBounds(chunk_coord.x, chunk_coord.y)){
        return;
    }

    Chunk* chunk;

    while(!queue.empty()){

        LightTile light_tile = queue.front();    
        queue.pop();

        if(light_tile.intensity < 0.05f){
            continue;
        }

        if(light_tile.coord.x == world->GetWorldProfile()->tilemap_profile.width){
            light_tile.coord.x = 0;
            light_tile.chunk_origin.x++;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
            //ResetClosedTileVectorForChunk(light_tile.chunk_origin);
        }
        else if(light_tile.coord.x == -1){
            light_tile.coord.x = world->GetWorldProfile()->tilemap_profile.width - 1;
            light_tile.chunk_origin.x--;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
            //ResetClosedTileVectorForChunk(light_tile.chunk_origin);
        }

        if(light_tile.coord.y == world->GetWorldProfile()->tilemap_profile.height){
            light_tile.coord.y = 0;
            light_tile.chunk_origin.y++;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
            //ResetClosedTileVectorForChunk(light_tile.chunk_origin);
        }
        else if(light_tile.coord.y == -1){
            light_tile.coord.y = world->GetWorldProfile()->tilemap_profile.height - 1;
            light_tile.chunk_origin.y--;

            if(!CanPropogateToNewChunk(light_tile.chunk_origin)){
                continue;
            }
            //ResetClosedTileVectorForChunk(light_tile.chunk_origin);
        }


        chunk = chunks->at(light_tile.chunk_origin.x).at(light_tile.chunk_origin.y);


        
        /*
        if(chunk->lighting_closed_grid[light_tile.coord.x][light_tile.coord.y] >= light_tile.intensity){
            continue;
        }
        */

        signed_byte current_tile = chunk->GetTilemap(SetLocation::MAIN)->GetTile(light_tile.coord.x, light_tile.coord.y);



        sf::Image& light_map = chunk->GetLightmap();
        
        sf::Color old_col = light_map.getPixel(light_tile.coord.x, light_tile.coord.y);
        sf::Color new_col(std::fmax(old_col.r, _colour.r * light_tile.intensity), std::fmax(old_col.g, _colour.g * light_tile.intensity),std::fmax(old_col.b, _colour.b * light_tile.intensity));

    
        if(old_col.r >= new_col.r && old_col.g >= new_col.g && old_col.b >= new_col.b){
            continue;
        }
    
        //chunk->lighting_closed_grid[light_tile.coord.x][light_tile.coord.y] = light_tile.intensity;
        light_map.setPixel(light_tile.coord.x, light_tile.coord.y, new_col);
        chunk->MarkLightmapDirty();

        if(current_tile != -1){
            light_tile.intensity *= 0.2f;
        }

        queue.push({light_tile.coord + sf::Vector2i(0, 1), light_tile.intensity - decay, light_tile.chunk_origin});
        queue.push({light_tile.coord + sf::Vector2i(0, -1), light_tile.intensity - decay, light_tile.chunk_origin });
        queue.push({light_tile.coord + sf::Vector2i(-1,0), light_tile.intensity - decay, light_tile.chunk_origin });
        queue.push({light_tile.coord + sf::Vector2i(1,0), light_tile.intensity - decay, light_tile.chunk_origin });

        chunk->lighting_closed_grid[light_tile.coord.x][light_tile.coord.y] = light_tile.intensity;
    }

    //Time::EndRecord();
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

