#pragma once
#include <SFML/Graphics.hpp>
#include "../Utility/Mouse.h"

class Scene;
class Object;

class RenderManager{

    enum RenderPasses{
        LIGHTING,
        LIGHTING_OFFSCREEN,
        LIGHTING_BLURRED,
        LIGHTING_POINTS,
        SCENE,
        SCENE_OFFSCREEN,
        COMPOSITE, // mixing lighting and scene
        NUMBER_OF_RENDER_PASSES // signifies the end
    };

    public:
        static void Construct();
        static void Destruct();
        
        static void Render(sf::RenderTarget&, Scene*);

        static void OnResize();

    private:
        static void ClearRenderTextures();

        static void DrawSceneObjectArray(std::vector<Object*>* array, Scene* scene, int render_layer);
        static void DrawFPS(sf::RenderTarget& surface);

        static void RenderLayer(sf::RenderTarget&, std::vector<Object*>& objects_at_layer, bool render_at_window_size = false, int render_layer = 0);
        static void RenderDebug(sf::RenderTarget&, std::vector<Object*>* array);
        static void RenderUI(sf::RenderTarget&, Scene*);



        static std::vector<sf::RenderTexture*> render_textures;
        
        static sf::Text fps_text;
        static int fps_refresh_delay_tracked;
        static int fps_refresh_delay;


};