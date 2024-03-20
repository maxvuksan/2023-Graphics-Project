#pragma once
#include "../../Amber/Framework.h"
#include "../Items/ItemDictionary.h"

class Player : public Object{

    public:

        void Start() override;
        void Update();

        sf::Vector2f GetPlayerRenderTexturePixelStep(){return texture_pixel_step;}
        sf::RenderTexture& GetPlayerRenderTexture(){return player_render_texture;}

        ItemCode item_in_hand;

    private:

        struct HairSegment{
            int radius;
            sf::Vector2f velocity;
            sf::Vector2f position;
        };

        Object* animator_object;

        int gap_between_hair;
        sf::CircleShape hair_circle;
        std::vector<HairSegment> hair_segments;

        AnimationRenderer* animator;
        sf::RenderTexture player_render_texture;
        sf::RenderTexture back_render_texture;
        sf::Vector2f texture_pixel_step;

};