#include "RenderManager.h"
#include "Core.h"
#include "../Object/Rendering/PointLight.h"
#include "../Object/Rendering/Tilemap.h"
#include <math.h>


std::vector<sf::RenderTexture*> RenderManager::render_textures;
//int RenderManager::camera_smoothing_edge_buffer = 1;

sf::Text RenderManager::fps_text;
int RenderManager::fps_refresh_delay_tracked;
int RenderManager::fps_refresh_delay = 150;


void RenderManager::Construct() {
    
    render_textures.resize(NUMBER_OF_RENDER_PASSES);

    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i] = Memory::New<sf::RenderTexture>(__FUNCTION__);
    }
    OnResize();

    sf::Font* font = AssetManager::GetFont("Amber_Default");
    fps_text.setFont(*font);
    fps_text.setCharacterSize(8);
    fps_text.setPosition(sf::Vector2f(Core::GetWindowWidth() - 100, 8));
    
}

void RenderManager::OnResize(){
    for(int i = 0; i < render_textures.size(); i++){
        //render_textures[i]->create(Core::GetDisplayWidth() + camera_smoothing_edge_buffer * 2, Core::GetDisplayHeight() + camera_smoothing_edge_buffer * 2);
        render_textures[i]->create(Core::GetDisplayWidth(), Core::GetDisplayHeight());
    }
}

void RenderManager::ClearRenderTextures(){
    // reset all render textures
    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i]->clear();
    }
}

void RenderManager::Render(sf::RenderTarget& surface, Scene* scene){

    ClearRenderTextures();

    // assign blur shader uniforms
    AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)render_textures[0]->getSize().x,
                    1 / (float)render_textures[0]->getSize().y));
    AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_strength", Globals::TILEMAP_SHADOW_BLUR);

    AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)render_textures[0]->getSize().x,
                    1 / (float)render_textures[0]->getSize().y));
    AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_strength", Globals::TILEMAP_SHADOW_BLUR);


    // clear scene texture
    //render_textures[SCENE_OFFSCREEN]->clear(Scene::GetActiveCamera()->background_colour);
    render_textures[SCENE]->clear(Scene::GetActiveCamera()->background_colour);

    sf::Sprite* background_sprite = Scene::GetActiveCamera()->GetBackgroundSprite();

    // fetch the cameras background sprite, only draw if is valid
    if(background_sprite != nullptr){
        background_sprite->setPosition(sf::Vector2f(render_textures[0]->getSize().x / 2.0f, render_textures[0]->getSize().y / 2.0f));
        render_textures[SCENE]->draw(*background_sprite);
    }

    // draw all objects
    std::vector<int>* render_layers = scene->GetRenderLayers();
    for(int i = 0; i < render_layers->size(); i++){
        DrawSceneObjectArray(scene->GetThisObjects(), scene, render_layers->at(i));
        DrawSceneObjectArray(scene->GetThisObjectsAdditional(), scene, render_layers->at(i));
    }

    // we have drawn our scene
    render_textures[SCENE]->display();

    sf::Sprite scene_sprite(sf::Sprite(render_textures[SCENE]->getTexture()));
    // translate our by its sub integer values (e.g. 0.5) to ensure a smooth camera movement
    //scene_sprite.setPosition(ceil(Scene::GetActiveCamera()->GetBoundedPosition().x) - Scene::GetActiveCamera()->GetBoundedPosition().x,
    //                            ceil(Scene::GetActiveCamera()->GetBoundedPosition().y) - Scene::GetActiveCamera()->GetBoundedPosition().y);
    render_textures[COMPOSITE]->draw(scene_sprite);


    // render debug graphics
    if(Core::DEBUG_MODE){
        RenderDebug(*render_textures[COMPOSITE], scene->GetThisObjects());
        RenderDebug(*render_textures[COMPOSITE], scene->GetThisObjectsAdditional());
    }

    // reusing a render texture to draw ui_overlay_colour
    render_textures[LIGHTING]->clear(Scene::GetActiveCamera()->ui_overlay_colour);
    render_textures[COMPOSITE]->draw(sf::Sprite(render_textures[LIGHTING]->getTexture()), sf::BlendAlpha);

    // draw all ui
    auto ui = scene->GetUI();
    auto ui_additional = scene->GetUIAdditional();

    std::vector<int>* ui_render_layers = scene->GetUIRenderLayers();
    
    for(int i = 0; i < ui_render_layers->size(); i++){
        RenderLayer(*render_textures[COMPOSITE], *ui, false, ui_render_layers->at(i));
        RenderLayer(*render_textures[COMPOSITE], *ui_additional, false, ui_render_layers->at(i));
    }

    // we have drawn everything in the "Display"
    render_textures[COMPOSITE]->display();


    // rescaling our final image to fit window
    sf::Sprite final_image = sf::Sprite(render_textures[COMPOSITE]->getTexture());
    final_image.setScale(sf::Vector2f(Core::GetWindowWidth() / (float)Core::GetDisplayWidth(), Core::GetWindowHeight() / (float)Core::GetDisplayHeight()));
    
    surface.draw(final_image); 

    // UI elements which are configured to draw at the window size
    RenderLayer(surface, *ui, true);

    DrawFPS(surface);

}

void RenderManager::DrawFPS(sf::RenderTarget& surface){
    if(Core::SHOW_FPS){

        // prevent flickering when fps changes
        if(fps_refresh_delay_tracked > fps_refresh_delay){
            fps_refresh_delay_tracked = 0;
            fps_text.setString("FPS: " + std::to_string(1000 / Time::Dt()));

        }
        fps_refresh_delay_tracked += Time::Dt();
        surface.draw(fps_text);
    }
}

void RenderManager::DrawSceneObjectArray(std::vector<Object*>* array, Scene* scene, int render_layer){

    bool light_found = false;

    // clear lighting texture for this layer
    render_textures[LIGHTING]->clear(sf::Color::White);
    //render_textures[LIGHTING_POINTS]->clear(Globals::BASE_SHADOW_COLOUR);

    for(auto obj : *array){

        if(!obj->IsActive()){
            continue;
        }

        // if it has a tilemap, we must consider lighting conditions
        Tilemap* tilemap = obj->GetComponent<Tilemap>();
        if(tilemap != nullptr && tilemap->GetRenderLayer() == render_layer){
            tilemap->DrawTilemapShadow(*render_textures[LIGHTING]);
        }
    /*
        for(auto light : *scene->GetPointLights()){

            // only draw light if it is on this layer
            if(light->GetThisObject()->GetRenderLayer() == render_layer){ 
                light->DrawLight(*render_textures[LIGHTING_POINTS]);
                light_found = true;
            }
        }
    */
        // draw objects and components
        for(auto comp : *obj->GetComponents()){
            
            if(!comp->IsActive() || comp->GetRenderLayer() != render_layer){
                continue;
            }
            comp->Draw(*render_textures[SCENE]);       
        }

        if(obj->GetRenderLayer() == render_layer){
            obj->Draw(*render_textures[SCENE]);       
        }
    }

    //if(!light_found){
    //    render_textures[LIGHTING_POINTS]->clear(sf::Color::White);
    //}

    render_textures[LIGHTING]->display();
    render_textures[SCENE_OFFSCREEN]->display();

    render_textures[LIGHTING_OFFSCREEN]->draw(sf::Sprite(render_textures[LIGHTING]->getTexture()), AssetManager::GetShader("Amber_BlurHorizontal"));
    render_textures[LIGHTING_OFFSCREEN]->display();

    render_textures[LIGHTING_BLURRED]->draw(sf::Sprite(render_textures[LIGHTING_OFFSCREEN]->getTexture()), AssetManager::GetShader("Amber_BlurVertical"));
    
    render_textures[LIGHTING_BLURRED]->display();
    //render_textures[LIGHTING_POINTS]->display(); 

    render_textures[SCENE]->draw(sf::Sprite(render_textures[LIGHTING_BLURRED]->getTexture()), sf::BlendMultiply);

}

void RenderManager::RenderDebug(sf::RenderTarget& surface, std::vector<Object*>* array){

    // marker for each objects position...
    sf::CircleShape debug_circle;
    debug_circle.setRadius(1);
    debug_circle.setFillColor(Globals::DEBUG_COLOUR);


    for(auto obj : *array){

        debug_circle.setPosition(Camera::WorldToScreenPosition(obj->GetTransform()->position));
        
        surface.draw(debug_circle);

        if(!obj->IsActive()){
            continue;
        }

        for(auto comp : *obj->GetComponents()){
            comp->DrawDebug(surface);       
        }
        obj->DrawDebug(surface);
    }

}


void RenderManager::RenderLayer(sf::RenderTarget& surface, std::vector<Object*>& objects_at_layer, bool render_at_window_size, int render_layer){

    for(auto obj : objects_at_layer){

        if(!obj->IsActive()){
            continue;
        }
        if(obj->GetRenderAtWindowSize() != render_at_window_size || render_layer != obj->GetRenderLayer()){
            continue;
        }

        for(auto comp : *obj->GetComponents()){

            if(!comp->IsActive() || render_layer != comp->GetRenderLayer()){
                continue;
            }
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