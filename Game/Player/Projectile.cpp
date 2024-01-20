#include "Projectile.h"
#include "../World/RotatedRectManager.h"

void Projectile::Start(){

    animation_renderer = AddComponent<AnimationRenderer>();
    animation_renderer->SetAnimationSet("tool_slash");
    
    rotating_rect = AddComponent<RotatingRect>();
    rotating_rect->SetSize(20, 16);
    //RotatedRectManager::AddRect(rotating_rect);

    speed = 0.15;
    inital_speed = speed;
    decay_speed = 0.008;
}

void Projectile::Update(){

    GetTransform()->position += sf::Vector2f(cos(Calc::Radians(GetTransform()->rotation)), sin(Calc::Radians(GetTransform()->rotation))) * Time::Dt() * speed;
    speed -= decay_speed;

    if(speed < 0){
        Destroy();
        RotatedRectManager::RemoveRect(rotating_rect);
    }

}