#include "Player.h"
#include <cmath>

void Player::Start(){

    GetScene()->SetActiveCamera(AddComponent<Camera>());

    auto col = AddComponent<BoxCollider>();
    col->SetSize(sf::Vector2f(8, 10));
    col->SetOffset(sf::Vector2f(-4,-5));

    ground = AddComponent<BoxCollider>();
    ground->SetIsTrigger(true);
    ground->SetSize(sf::Vector2f(8, 3));
    ground->SetOffset(sf::Vector2f(-4,6));

    left = AddComponent<BoxCollider>();
    left->SetIsTrigger(true);
    left->SetSize(sf::Vector2f(2, 8));
    left->SetOffset(sf::Vector2f(-8,-4));

    right = AddComponent<BoxCollider>();
    right->SetIsTrigger(true);
    right->SetSize(sf::Vector2f(2, 8));
    right->SetOffset(sf::Vector2f(6,-4));


    pb = AddComponent<PhysicsBody>();
    AddComponent<PointLight>();
}

void Player::Update(){


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if(pb->velocity.x > -0.2){
            pb->velocity.x -= 0.008f;
        }
        else{
            pb->velocity.x = -0.2;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if(pb->velocity.x < 0.2){
            pb->velocity.x += 0.008f;
        }
        else{
            pb->velocity.x = 0.2;
        }
    }
    else{
        if(fabsf(pb->velocity.x) > 0.001){
            if(pb->velocity.x > 0){
                pb->velocity.x -= 0.005;
            }
            else{
                pb->velocity.x += 0.005;
            }
        }
        else{
            pb->velocity.x = 0;
        }
    }
    
}

void Player::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.scancode == sf::Keyboard::Scan::Z){
       
            // only allow jumping when we have ground below
            if(ground->Triggered()){
                pb->velocity.y = -jump_height;
            }
            else if(left->Triggered()){
                pb->velocity.y = -jump_height;
                pb->velocity.x = 0.17f;
            }
            else if(right->Triggered()){
                pb->velocity.y = -jump_height;
                pb->velocity.x = -0.17f;
            }
        }       
    }
}
