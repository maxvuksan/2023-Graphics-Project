#pragma once
#include <SFML/Graphics.hpp>
#include "../Utility/Mouse.h"

class Core;
class Scene;

class RenderManager{

    enum RenderPasses{
        LIGHTING,
        LIGHTING_BLURRED,
        LIGHTING_FINAL,
        SCENE,
        COMPOSITE, // mixing lighting and scene
        NUMBER_OF_RENDER_PASSES // signifies the end
    };

    public:
        static void Construct(Core* core);
        static void Destruct();
        
        static void Render(sf::RenderTarget& surface, Scene*);

    private:
        static Core* core;
        static std::vector<sf::RenderTexture*> render_textures;
  
};