#pragma once
#include "../Component.h"

class PointLight : public Component{

    public:

        PointLight();

        void Start() override;
        ~PointLight() override;

        void DrawLight(sf::RenderTarget& surface);

        void SetColour(sf::Color colour);
        void SetRadius(float radius);
        void SetStrength(float strength);
    
    private:
        
        void DrawLightRenderTexture();

        sf::Color colour;
        float radius;
        float strength; //is clamped between 0-1
        float _strength; 

        sf::Sprite light_sprite;
        sf::Shader* point_shader;
        sf::RenderTexture texture;
};