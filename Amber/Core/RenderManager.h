#pragma once
#include <SFML/Graphics.hpp>

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
        RenderManager(Core* core);

        void Render(sf::RenderTarget& surface, Scene*);

        ~RenderManager();
    private:
        Core* core;
        std::vector<sf::RenderTexture*> render_textures;
  
};