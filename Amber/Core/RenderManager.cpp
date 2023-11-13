#include "RenderManager.h"
#include "Core.h"
#include "../Object/Rendering/PointLight.h"
#include "../Object/Rendering/Tilemap.h"
#include <math.h>


std::vector<sf::RenderTexture*> RenderManager::render_textures;

void RenderManager::Construct() {
    
    render_textures.resize(NUMBER_OF_RENDER_PASSES);

    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i] = Memory::New<sf::RenderTexture>(__FUNCTION__);
    }

}

void RenderManager::ClearRenderTextures(){
    // reset all render textures
    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i]->create(Core::GetDisplayWidth(), Core::GetDisplayHeight());
        render_textures[i]->clear();
    }
}

void RenderManager::Render(sf::RenderTarget& surface, Scene* scene){

    ClearRenderTextures();

    auto objects = scene->GetObjects();
    
    // clear scene texture
    render_textures[SCENE_OFFSCREEN]->clear(Scene::GetActiveCamera()->background_colour);

    // setup blur shader
    auto blur_hor_shader = AssetManager::GetShader("Amber_BlurHorizontal");
    auto blur_vert_shader = AssetManager::GetShader("Amber_BlurVertical");

    blur_hor_shader->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)Core::GetDisplayWidth(),
                    1 / (float)Core::GetDisplayHeight()));
    blur_hor_shader->setUniform("u_strength", Globals::TILEMAP_SHADOW_BLUR);

    blur_vert_shader->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)Core::GetDisplayWidth(),
                    1 / (float)Core::GetDisplayHeight()));
    blur_vert_shader->setUniform("u_strength", Globals::TILEMAP_SHADOW_BLUR);


    for (auto layer = objects->begin(); layer != objects->end(); layer++) {
        
        // clear lighting texture for this layer
        render_textures[LIGHTING]->clear(sf::Color::White);
        render_textures[LIGHTING_POINTS]->clear(Globals::BASE_SHADOW_COLOUR);
        
        bool light_found = false;

        for(auto obj : layer->second){

            if(!obj->IsActive()){
                continue;
            }

            // if it has a tilemap, we must consider lighting conditions
            Tilemap* tilemap = obj->GetComponent<Tilemap>();
            if(tilemap != nullptr){
                tilemap->Draw_EdgeLighting(*render_textures[LIGHTING]);
            }

            for(auto light : *scene->GetPointLights()){

                // only draw light if it is on this layer
                if(light->GetObject()->GetRenderLayer() == layer->first){
                    light_found = true;
                    light->DrawLight(*render_textures[LIGHTING_POINTS]);
                }
            }

            // draw objects and components
            for(auto comp : *obj->GetComponents()){
                comp->Draw(*render_textures[SCENE_OFFSCREEN]);       
            }
            obj->Draw(*render_textures[SCENE_OFFSCREEN]);       
        }

        if(!light_found){
            render_textures[LIGHTING_POINTS]->clear(sf::Color::White);
        }

        render_textures[LIGHTING]->display();
        render_textures[SCENE_OFFSCREEN]->display();


        render_textures[LIGHTING_OFFSCREEN]->draw(sf::Sprite(render_textures[LIGHTING]->getTexture()), blur_hor_shader);
        render_textures[LIGHTING_OFFSCREEN]->display();

        render_textures[LIGHTING_BLURRED]->draw(sf::Sprite(render_textures[LIGHTING_OFFSCREEN]->getTexture()), blur_vert_shader);
        
        render_textures[LIGHTING_BLURRED]->display();
        render_textures[LIGHTING_POINTS]->display(); 

        render_textures[SCENE]->draw(sf::Sprite(render_textures[LIGHTING_BLURRED]->getTexture()));
        //render_textures[SCENE]->draw(sf::Sprite(render_textures[LIGHTING_POINTS]->getTexture()), sf::BlendMultiply);
        render_textures[SCENE]->draw(sf::Sprite(render_textures[SCENE_OFFSCREEN]->getTexture()), sf::BlendMultiply);
    }
    // we have drawn our scene
    render_textures[SCENE]->display();

    render_textures[COMPOSITE]->draw(sf::Sprite(render_textures[SCENE]->getTexture()));


    // render debug graphics
    if(Core::DEBUG_MODE){
        RenderDebug(*render_textures[COMPOSITE], scene);
    }


    auto ui = scene->GetUI();
    for (auto layer = ui->begin(); layer != ui->end(); layer++) {
        RenderLayer(*render_textures[COMPOSITE], layer->second);
    }

    render_textures[COMPOSITE]->display();


    // rescaling our final image to fit window
    sf::Sprite final_image = sf::Sprite(render_textures[COMPOSITE]->getTexture());
    final_image.setScale(sf::Vector2f(Core::GetWindowWidth() / (float)Core::GetDisplayWidth(), Core::GetWindowHeight() / (float)Core::GetDisplayHeight()));
    
    surface.draw(final_image); 

    // UI elements which are configured to render at the window size
    for (auto layer = ui->begin(); layer != ui->end(); layer++) {
        RenderLayer(surface, layer->second, true);
    }

}

void RenderManager::RenderDebug(sf::RenderTarget& surface, Scene* scene){
    auto objects = scene->GetObjects();

    // marker for each objects position...
    sf::CircleShape debug_circle;
    debug_circle.setRadius(1);
    debug_circle.setFillColor(Globals::DEBUG_COLOUR);

    for (auto layer = objects->begin(); layer != objects->end(); layer++) {

        for(auto obj : layer->second){

            debug_circle.setPosition(Camera::WorldToScreenPosition(obj->GetTransform()->position));
            
            surface.draw(debug_circle);

            for(auto comp : *obj->GetComponents()){
                comp->Draw_Debug(surface);       
            }
            obj->Draw_Debug(surface);
        }
    }

}


void RenderManager::RenderLayer(sf::RenderTarget& surface, std::vector<Object*>& objects_at_layer, bool render_at_window_size){

    for(auto obj : objects_at_layer){

        if(!obj->IsActive()){
            continue;
        }
        if(obj->GetRenderAtWindowSize() != render_at_window_size){
            continue;
        }

        for(auto comp : *obj->GetComponents()){
            comp->Draw(surface);       
        }
        obj->Draw(surface);       
    }

}

void RenderManager::Destruct(){
    for(int i = 0; i < render_textures.size(); i++){
        Memory::Delete<sf::RenderTexture>(render_textures[i], __FUNCTION__);
    }
}