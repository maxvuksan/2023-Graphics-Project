#include "Player.h"
#include "../World/Lighting/LightSource.h"
#include "../Items/ItemDictionary.h"
#include "../World/Lighting/LightingManager.h"

void Player::Start(){

    player_render_texture.create(100, 100);
    back_render_texture.create(100, 100);
    texture_pixel_step = sf::Vector2f(1 / (float)player_render_texture.getSize().x,
                            1 / (float)player_render_texture.getSize().y);

    AddComponent<PhysicsBody>();

    animator = AddComponent<AnimationRenderer>();
    animator->SetAnimationSet("player");
    animator->SetUseFixedPosition(true);
    animator->SetActive(false);
    
    SetRenderLayer(8);

    auto body_collider = AddComponent<BoxCollider>();
    body_collider->SetSize(sf::Vector2f(10, 18));
    body_collider->SetOffset(sf::Vector2f(-5,-8));
    
    //AddComponent<PixelParticleSource>();



    // hair segmenets _______________

    hair_segments.push_back({
        5,
        sf::Vector2f(0,0),
        sf::Vector2f(50,48),
    });
    hair_segments.push_back({
        5,
        sf::Vector2f(0,0),
    });
    hair_segments.push_back({
        4,
        sf::Vector2f(0,0),
    });
    hair_segments.push_back({
        4,
        sf::Vector2f(0,0),
    });
    hair_segments.push_back({
        3,
        sf::Vector2f(0,0),
    });

    hair_circle.setFillColor(sf::Color(104, 60, 44));
    gap_between_hair = 2;
}

void Player::Update(){


    if(item_in_hand == ItemCode::item_Torch){
        
        // TO DO : Add player visual component, allows player components to talk to each other and is responsible for players appearnace and item in hand
        
        //LightingManager::PropogateLighting(,ItemDictionary::torch_colour, ItemDictionary::torch_propogate_decay);
    }

    player_render_texture.clear(sf::Color::Transparent);
    back_render_texture.clear(sf::Color::Transparent);

    for(int i = 0; i < hair_segments.size(); i++){

        hair_circle.setRadius(hair_segments[i].radius);
        hair_circle.setOrigin(sf::Vector2f(hair_segments[i].radius, hair_segments[i].radius));
        hair_circle.setPosition(hair_segments[i].position);
    
        back_render_texture.draw(hair_circle);

        if(i > 0){

            float distance_to_previous = Calc::Distance(hair_segments[i].position, hair_segments[i - 1].position);


            hair_segments[i].velocity = hair_segments[i].velocity * 0.4f + GetComponent<PhysicsBody>()->velocity * -1.0f;

            hair_segments[i].velocity.x = Calc::Clamp(hair_segments[i].velocity.x, -1, 1);
            hair_segments[i].velocity.y = Calc::Clamp(hair_segments[i].velocity.y, -1, 1);

            hair_segments[i].position += hair_segments[i].velocity * Time::Dt();

            sf::Vector2f direction = Calc::VectorBetween(hair_segments[i - 1].position, hair_segments[i].position);

            if(distance_to_previous >= gap_between_hair - 1){
                hair_segments[i].position = hair_segments[i - 1].position + direction * (float)gap_between_hair;
                hair_segments[i].velocity.x = 0;
                hair_segments[i].velocity.y = 0;
            }
            
        }
    }

    back_render_texture.display();

    //AssetManager::GetShader("Amber_OutlineOuter")->setUniform("u_texture_pixel_step", GetPlayerRenderTexturePixelStep());
    //AssetManager::GetShader("Amber_OutlineOuter")->setUniform("u_colour", sf::Glsl::Vec4(0.0, 0.0, 0.0, 1.0));
    player_render_texture.draw(sf::Sprite(back_render_texture.getTexture()));//, AssetManager::GetShader("Amber_OutlineOuter"));

    animator->Update();
    animator->SetFixedPosition(sf::Vector2f(50,50));
    animator->Draw(player_render_texture);

    player_render_texture.display();



}
