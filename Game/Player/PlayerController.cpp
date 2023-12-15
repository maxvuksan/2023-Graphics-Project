#include "PlayerController.h"
#include "../World/World.h"
#include "../Items/ItemDictionary.h"

float PlayerController::jump_height = 0.175;
float PlayerController::jump_buffer = 100;
float PlayerController::cyote_time_buffer = 100;

void PlayerController::Start(){
    
    // set player camera as scene camera
    object->GetScene()->SetActiveCamera(object->AddComponent<Camera>());
    // set still background

    body_collider = object->AddComponent<BoxCollider>();
    body_collider->SetSize(sf::Vector2f(8, 16));
    body_collider->SetOffset(sf::Vector2f(-4,-8));

    ground = object->AddComponent<BoxCollider>();
    ground->SetIsTrigger(true);
    ground->SetSize(sf::Vector2f(8, 4));
    ground->SetOffset(sf::Vector2f(-4,8));

    left = object->AddComponent<BoxCollider>();
    left->SetIsTrigger(true);
    left->SetSize(sf::Vector2f(2, 8));
    left->SetOffset(sf::Vector2f(-6,-4));

    right = object->AddComponent<BoxCollider>();
    right->SetIsTrigger(true);
    right->SetSize(sf::Vector2f(2, 8));
    right->SetOffset(sf::Vector2f(6,-4));
    
    pb = object->AddComponent<PhysicsBody>();

    animation = object->GetComponent<AnimationRenderer>();

    in_fly_mode = false;

    jump_buffer_tracked = 0;
    cyote_time_buffer_tracked = 0;

}


void PlayerController::UpdateEventFocusBounded(){

    float speed = 0.05;

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
        animation->SetFlip(true);
        animation->SetState("run");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        pb->velocity.x = speed;
        animation->SetFlip(false);
        animation->SetState("run");
    }
    else{
        pb->velocity.x = 0;      
        animation->SetState("idle"); 
    }

    if(!ground->Triggered()){

        if(left->Triggered()){
            animation->SetState("onwall");
            animation->SetFlip(true);
        }
        else if(right->Triggered()){
            animation->SetState("onwall");
            animation->SetFlip(false);
        }
        else {
            if(pb->velocity.y < 0){
                animation->SetState("fallup"); 
            }
            else {
                animation->SetState("falldown"); 
            }
        }

    }
    else{ // is grounded
        cyote_time_buffer_tracked = cyote_time_buffer;
    }



    if(jump_buffer_tracked > 0){

        if(cyote_time_buffer_tracked > 0){
            Jump();
            jump_buffer_tracked = 0;
        }
    }

    if(jump_buffer_tracked >= 0){
        jump_buffer_tracked -= Time::Dt();
    }
    if(cyote_time_buffer_tracked >= 0){
        cyote_time_buffer_tracked -= Time::Dt();
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
                if(cyote_time_buffer_tracked > 0){
                    Jump();
                }
                else{
                    // hold onto jump
                    jump_buffer_tracked = jump_buffer;
                }


               
                break;
            }       
        }
        
    }       
}

void PlayerController::Jump(){

    pb->velocity.y = -jump_height;

    /* WALL JUMP----
    else if(left->Triggered()){

        float half_jump = jump_height / 2.0f;
        pb->velocity.y = -half_jump;
        pb->velocity.x = half_jump; 
    }
    else if(right->Triggered()){

        float half_jump = jump_height / 2.0f;
        pb->velocity.y = -half_jump;
        pb->velocity.x = -half_jump; 
    }
    */

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