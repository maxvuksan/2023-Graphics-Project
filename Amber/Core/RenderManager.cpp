#include "RenderManager.h"
#include "Core.h"
#include "../Object/Rendering/Tilemap.h"
#include <math.h>


std::vector<sf::RenderTexture*> RenderManager::render_textures;
int RenderManager::camera_smoothing_edge_buffer = 4;

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
        render_textures[i]->create(Core::GetDisplayWidth() + camera_smoothing_edge_buffer * 2, Core::GetDisplayHeight() + camera_smoothing_edge_buffer * 2);
        //render_textures[i]->create(Core::GetDisplayWidth(), Core::GetDisplayHeight());
    }

    // assign blur shader uniforms
    AssetManager::GetShader("Amber_BlurHorizontal")->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)render_textures[0]->getSize().x,
                    1 / (float)render_textures[0]->getSize().y));

    AssetManager::GetShader("Amber_BlurVertical")->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)render_textures[0]->getSize().x,
                    1 / (float)render_textures[0]->getSize().y));
}

void RenderManager::ClearRenderTextures(){
    // reset all render textures
    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i]->clear();
    }
}

void RenderManager::Render(sf::RenderTarget& surface, Scene* scene){

    ClearRenderTextures();

    // clear scene texture

    sf::Sprite* background_sprite = Scene::GetActiveCamera()->GetBackgroundSprite();

    // fetch the cameras background sprite, only draw if is valid
    if(background_sprite != nullptr){
        render_textures[SCENE]->clear(sf::Color::Transparent);
    }
    else{
        render_textures[SCENE]->clear(sf::Color::Transparent);
    }

    // draw all objects
    std::vector<int>* render_layers = scene->GetRenderLayers();
    for(int i = 0; i < render_layers->size(); i++){
        DrawSceneObjectArray(scene->GetThisObjects(), scene, render_layers->at(i));
        DrawSceneObjectArray(scene->GetThisObjectsAdditional(), scene, render_layers->at(i));
    }

    // we have drawn our scene
    render_textures[SCENE]->display();

    // draw backgorund if necassary
    if(background_sprite != nullptr){
        background_sprite->setPosition(sf::Vector2f(render_textures[0]->getSize().x / 2.0f, render_textures[0]->getSize().y / 2.0f));
        background_sprite->setColor(Scene::GetActiveCamera()->background_colour);
        render_textures[COMPOSITE]->draw(*background_sprite);
    }
    render_textures[COMPOSITE]->display();

    // rescaling our final image to fit window
    sf::Sprite upscaled_image = sf::Sprite(render_textures[COMPOSITE]->getTexture());
    upscaled_image.setScale(sf::Vector2f(Core::GetDisplayToWindowMultiplier().x, Core::GetDisplayToWindowMultiplier().y));
    surface.draw(upscaled_image);

    // render debug graphics
    if(Core::DEBUG_MODE){
        RenderDebug(*render_textures[SCENE], scene->GetThisObjects());
        RenderDebug(*render_textures[SCENE], scene->GetThisObjectsAdditional());
    }
    render_textures[SCENE]->display();
    
    // smoothing camera movement
    sf::Vector2f cam_pos = Scene::GetActiveCamera()->GetBoundedPosition();
    upscaled_image.setPosition(sf::Vector2f(
        -camera_smoothing_edge_buffer,//-(cam_pos.x - floor(cam_pos.x)) * Core::GetDisplayToWindowMultiplier().x, 
        -camera_smoothing_edge_buffer));//-(cam_pos.y - floor(cam_pos.y)) * Core::GetDisplayToWindowMultiplier().y ) );
    
    upscaled_image.setTexture(render_textures[SCENE]->getTexture());
    surface.draw(upscaled_image);


    // reusing Scene render texture to draw ui_overlay_colour
    render_textures[SCENE]->clear(Scene::GetActiveCamera()->ui_overlay_colour);
    render_textures[SCENE]->display();

    render_textures[COMPOSITE]->clear(sf::Color::Transparent);
    render_textures[COMPOSITE]->draw(sf::Sprite(render_textures[SCENE]->getTexture()));

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

    upscaled_image.setTexture(render_textures[COMPOSITE]->getTexture());
    upscaled_image.setPosition(sf::Vector2f(-camera_smoothing_edge_buffer,-camera_smoothing_edge_buffer));
    surface.draw(upscaled_image); 

    // UI elements which are configured to draw at the window size
    RenderLayer(surface, *ui, true);

    DrawFPS(surface);

    // apply screen wipe if it is present
    if(Scene::GetActiveCamera()->GetFinalScreenWipeColour().a != 0){
        render_textures[COMPOSITE]->clear(sf::Color::White);
        render_textures[COMPOSITE]->display();

        upscaled_image.setTexture(render_textures[COMPOSITE]->getTexture());
        upscaled_image.setColor(Scene::GetActiveCamera()->GetFinalScreenWipeColour());

        surface.draw(upscaled_image);
    }

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

    for(auto obj : *array){

        if(!obj->IsActive()){
            continue;
        }

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