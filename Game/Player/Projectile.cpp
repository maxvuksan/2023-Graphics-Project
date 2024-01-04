#include "Projectile.h"

void Projectile::Start(){

    sr = AddComponent<SpriteRenderer>();
    sr->SetTexture("tool_slash", true);
    speed = 0.3;
    inital_speed = speed;
    decay_speed = 0.008;
}

void Projectile::Update(){

    GetTransform()->position += sf::Vector2f(cos(Calc::Radians(GetTransform()->rotation)), sin(Calc::Radians(GetTransform()->rotation))) * Time::Dt() * speed;
    speed -= decay_speed;


    sr->GetSprite()->setColor(sf::Color(255,255,255, 255 * speed / inital_speed));

    if(speed < 0){
        Destroy();
    }
}