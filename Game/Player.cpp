#include "Player.h"
#include "World.h"
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


    // Movement...

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        pb->velocity.x = -0.08;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        pb->velocity.x = 0.08;
    }
    else{
        pb->velocity.x = 0;       
    }

    CalculateMouse();
    
}

void Player::CalculateMouse(){

    sf::Vector2i _mouse_pos = Mouse::DisplayPosition();
    sf::Vector2f player_center = GetScene()->GetActiveCamera()->WorldToScreenPosition(GetTransform()->position);

    _mouse_pos.x -= round(player_center.x);
    _mouse_pos.y -= round(player_center.y);

    mouse_pos = sf::Vector2i(round(GetTransform()->position.x) + _mouse_pos.x, 
                             round(GetTransform()->position.y) + _mouse_pos.y );
}

void Player::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.scancode == sf::Keyboard::Scan::W){
       
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
    if(event.type == sf::Event::MouseButtonPressed){
        if(event.mouseButton.button == 0){
            
            world->SetTile_FromWorld(-1, mouse_pos.x, mouse_pos.y);
        }
    }
}

