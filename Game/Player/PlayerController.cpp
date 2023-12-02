#include "PlayerController.h"
#include "../World/World.h"
#include "../Items/ItemDictionary.h"

void PlayerController::Start(){
    
    // set player camera as scene camera
    object->GetScene()->SetActiveCamera(object->AddComponent<Camera>());
    // set still background

    body_collider = object->AddComponent<BoxCollider>();
    body_collider->SetSize(sf::Vector2f(6, 16));
    body_collider->SetOffset(sf::Vector2f(-3,-8));

    ground = object->AddComponent<BoxCollider>();
    ground->SetIsTrigger(true);
    ground->SetSize(sf::Vector2f(8, 4));
    ground->SetOffset(sf::Vector2f(-3,8));

    left = object->AddComponent<BoxCollider>();
    left->SetIsTrigger(true);
    left->SetSize(sf::Vector2f(2, 8));
    left->SetOffset(sf::Vector2f(-8,-4));

    right = object->AddComponent<BoxCollider>();
    right->SetIsTrigger(true);
    right->SetSize(sf::Vector2f(2, 6));
    right->SetOffset(sf::Vector2f(6,-3));
    
    pb = object->AddComponent<PhysicsBody>();

    in_fly_mode = false;
}

void PlayerController::UpdateEventFocusBounded(){

    float speed = 0.08;

    // Movement...

    if(in_fly_mode){

        // double speed in fly mode
        speed *= 2;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            pb->velocity.y = -speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            pb->velocity.y = speed;
        }
        else{
            pb->velocity.y = 0;       
        }
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        pb->velocity.x = -speed;
        object->GetComponent<SpriteRenderer>()->SetFlip(true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        pb->velocity.x = speed;
        object->GetComponent<SpriteRenderer>()->SetFlip(false);
    }
    else{
        pb->velocity.x = 0;       
    }
}


void PlayerController::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.scancode){

            case sf::Keyboard::Scan::W: {
        
                if(in_fly_mode){
                    break;
                }

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
                break;
            }       
        }
        
    }       
}

void PlayerController::Respawn(){
    object->GetTransform()->position = sf::Vector2f(50, -500);
}
void PlayerController::SetFlyMode(bool state){
    in_fly_mode = state;
    pb->SetGravityState(!state);
}
bool PlayerController::GetFlyMode(){
    return in_fly_mode;
}