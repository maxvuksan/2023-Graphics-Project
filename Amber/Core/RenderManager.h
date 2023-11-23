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

    private:
        static void ClearRenderTextures();
        static void RenderLayer(sf::RenderTarget&, std::vector<Object*>& objects_at_layer, bool render_at_window_size = false);
        static void RenderDebug(sf::RenderTarget&, Scene*);
        static void RenderUI(sf::RenderTarget&, Scene*);

        static std::vector<sf::RenderTexture*> render_textures;


};