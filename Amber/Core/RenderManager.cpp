#include "RenderManager.h"
#include "Core.h"
#include "../Object/Rendering/PointLight.h"
#include "../Object/Rendering/Tilemap.h"


std::vector<sf::RenderTexture*> RenderManager::render_textures;


void RenderManager::Construct() {
    
    render_textures.resize(NUMBER_OF_RENDER_PASSES);

    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i] = new sf::RenderTexture;
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
    render_textures[SCENE]->clear(Scene::GetActiveCamera()->background_colour);

    for (auto layer = objects->begin(); layer != objects->end(); layer++) {
        RenderLayer(*render_textures[SCENE], layer->second);
    }
    render_textures[SCENE]->display();


    render_textures[COMPOSITE]->draw(sf::Sprite(render_textures[SCENE]->getTexture()));



    // render debug graphics
    if(Core::DEBUG_MODE){
        RenderDebug(*render_textures[COMPOSITE], scene);
    }


    // render UI
    
    auto ui = scene->GetUI();
    for (auto layer = ui->begin(); layer != ui->end(); layer++) {
        RenderLayer(*render_textures[COMPOSITE], layer->second);
    }
    
    render_textures[COMPOSITE]->display();




    // rescaling our final image to fit window
    sf::Sprite final_image = sf::Sprite(render_textures[COMPOSITE]->getTexture());
    final_image.setScale(Core::GetDisplayToWindowMultiplier());

    surface.draw(final_image);


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


void RenderManager::RenderLayer(sf::RenderTarget& surface, std::vector<Object*>& objects_at_layer){

    for(auto obj : objects_at_layer){

        if(!obj->IsActive()){
            continue;
        }

        for(auto comp : *obj->GetComponents()){
            comp->Draw(surface);       
        }
        obj->Draw(surface);       
    }
}

/*

void RenderManager::Render(sf::RenderTarget& surface, Scene* scene){
    sf::Vector2i display_size = Core::GetDisplaySize();
    sf::Vector2i window_size = Core::GetWindowSize();
    sf::Shader* blur_shader = AssetManager::GetShader("Amber_Blur");

    // reset all render textures
    for(int i = 0; i < render_textures.size(); i++){
        render_textures[i]->create(display_size.x, display_size.y);
        render_textures[i]->clear();
    }

    // draw lighting______________________________________________
        
    render_textures[LIGHTING]->clear(Globals::BASE_SHADOW_COLOUR);

    for(auto light : *scene->GetPointLights()){
        if(!light->GetObject()->IsActive()){
            continue;
        }

        light->DrawLight(*render_textures[LIGHTING]);
    }
    
    for(auto obj : *scene->GetObjects()){
        if(!obj->IsActive()){
            continue;
        }

        for(auto comp : *obj->GetComponents()){
            comp->Draw_ShadowPass(*render_textures[LIGHTING]);       
        }
    }
    
    render_textures[LIGHTING]->display();
    // prepare for blurring lighting
    blur_shader->setUniform("u_strength", 2.0f);
    blur_shader->setUniform("u_texture", render_textures[LIGHTING]->getTexture());
    blur_shader->setUniform("u_texture_pixel_step", 
        sf::Vector2f(1 / (float)render_textures[LIGHTING]->getTexture().getSize().x,
                    1 / (float)render_textures[LIGHTING]->getTexture().getSize().y));

    // draw lighting blurred
    render_textures[LIGHTING_BLURRED]->draw(sf::Sprite(render_textures[LIGHTING]->getTexture()), blur_shader);
    
    for(auto obj : *scene->GetObjects()){
        if(!obj->IsActive()){
            continue;
        }

        for(auto comp : *obj->GetComponents()){
            comp->Draw_ShadowPass_PostBlur(*render_textures[LIGHTING_BLURRED]);       
        }
    }

    render_textures[LIGHTING_BLURRED]->display();


    // drawing edge lighting (for Tilemap objects)
    //  we can reuse the LIGHTING render texture because we are done with it
    render_textures[LIGHTING]->clear(sf::Color::White);
    for(auto tmap : *scene->GetTilemaps()){
        if(!tmap->GetObject()->IsActive()){
            continue;
        }

        tmap->Draw_EdgeLighting(*render_textures[LIGHTING]);
    }

    blur_shader->setUniform("u_strength", 3.0f);

    render_textures[LIGHTING_FINAL]->draw(sf::Sprite(render_textures[LIGHTING]->getTexture()), blur_shader);
    render_textures[LIGHTING_FINAL]->draw(sf::Sprite(render_textures[LIGHTING_BLURRED]->getTexture()), sf::BlendMultiply);
    render_textures[LIGHTING_FINAL]->display();

    // draw scene_________________________________________________

    render_textures[SCENE]->clear(scene->GetActiveCamera()->background_colour);
    for(auto obj : *scene->GetObjects()){
        if(!obj->IsActive()){
            continue;
        }

        for(auto comp : *obj->GetComponents()){
            comp->Draw(*render_textures[SCENE]);       
        }
        obj->Draw(*render_textures[SCENE]);       
    }
    render_textures[SCENE]->display();

    // draw lighting blurred
    render_textures[COMPOSITE]->draw(sf::Sprite(render_textures[LIGHTING_FINAL]->getTexture()));
    // draw scene in relation to lighting
    render_textures[COMPOSITE]->draw(sf::Sprite(render_textures[SCENE]->getTexture()), sf::BlendMultiply);

    // draw debug _______________________________________________
    if(Core::DEBUG_MODE){

        sf::CircleShape debug_circle;
        debug_circle.setRadius(1);
        debug_circle.setFillColor(Globals::DEBUG_COLOUR);

        for(auto obj : *scene->GetObjects()){

            debug_circle.setPosition(scene->GetActiveCamera()->WorldToScreenPosition(obj->GetTransform()->position));
            
            render_textures[COMPOSITE]->draw(debug_circle);

            for(auto comp : *obj->GetComponents()){
                comp->Draw_Debug(*render_textures[COMPOSITE]);       
            }
            obj->Draw_Debug(*render_textures[COMPOSITE]);
        }
    }

    // display final scene
    render_textures[COMPOSITE]->display();



    sf::Vector2i mouse_pos = Mouse::WindowPosition(); 
    Mouse::SetDisplayPosition(sf::Vector2i(mouse_pos.x * (display_size.x / (float)window_size.x), 
                                           mouse_pos.y * display_size.y / (float)window_size.y));

    // rescaling our final image to fit window
    sf::Sprite final_image = sf::Sprite(render_textures[COMPOSITE]->getTexture());
    final_image.setScale(sf::Vector2f(window_size.x / (float)display_size.x, window_size.y / (float)display_size.y));

    surface.draw(final_image);

    for(auto obj : *scene->GetObjects()){
        obj->Draw_Window(surface);
    }
}
*/
void RenderManager::Destruct(){
    for(int i = 0; i < render_textures.size(); i++){
        delete render_textures[i];
    }
}