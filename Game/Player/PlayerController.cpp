#include "PlayerController.h"
#include "../World/World.h"
#include "../Items/ItemDictionary.h"

float PlayerController::speed = 0.07;
float PlayerController::jump_height = 0.175;
float PlayerController::jump_buffer = 100;
float PlayerController::cyote_time_buffer = 100;
float PlayerController::cyote_time_wall = 100;
float PlayerController::climb_speed = 0.03;
float PlayerController::grab_delay = 200;
float PlayerController::wall_kickoff_force = 0.10;
float PlayerController::wall_jump_control_blend = 300;
float PlayerController::wall_kickoff_height = 0.15;

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
    cyote_time_left_wall_tracked = 0;
    cyote_time_right_wall_tracked = 0;
    wall_jump_control_blend_tracked = 0;
    grab_delay_tracked = 0;
    forced_velocity_x = 0;
    controlled_velocity_x = 0;

}


void PlayerController::UpdateEventFocusBounded(){

    float _speed = speed;

    // Movement...

    if(in_fly_mode){

        // double speed in fly mode
        _speed *= 2;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            pb->velocity.y = -_speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            pb->velocity.y = _speed;
        }
        else{
            pb->velocity.y = 0;       
        }
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if(!grabbing_wall || grabbing_wall_right){
            grabbing_wall = false;
            grabbing_wall_right = false;
            
            controlled_velocity_x = -_speed;
            animation->SetFlip(true);
            animation->SetState("run");
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if(!grabbing_wall || grabbing_wall_left){
            grabbing_wall = false;
            grabbing_wall_left = false;
            

            controlled_velocity_x = _speed;
            animation->SetFlip(false);
            animation->SetState("run");
        }
    }
    else{
        controlled_velocity_x = 0;      
        animation->SetState("idle"); 
    }

    pb->velocity.x = Calc::Lerp(controlled_velocity_x, forced_velocity_x, wall_jump_control_blend_tracked / wall_jump_control_blend);

    if(grabbing_wall){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            pb->velocity.y = climb_speed;
        }
        else{
            pb->velocity.y = 0;
            pb->gravity = 0;
        }
    }
    else{
        pb->gravity = 1;
    }

    if(!ground->Triggered()){

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && grab_delay_tracked <= 0){
            grabbing_wall = true;
        }
        else {
            grabbing_wall = false;
            grabbing_wall_left = false;
            grabbing_wall_right = false;
        }

        if(pb->velocity.y < 0){
            animation->SetState("fallup"); 
        }
        else {
            animation->SetState("falldown"); 
        }

        if(left->Triggered()){
            cyote_time_left_wall_tracked = cyote_time_wall;

            if(grabbing_wall){
                animation->SetState("onwall");
                animation->SetFlip(true);
                grabbing_wall_left = true;
            }

        }
        else if(right->Triggered()){
            cyote_time_right_wall_tracked = cyote_time_wall;

            if(grabbing_wall && !grabbing_wall_left){
                animation->SetState("onwall");
                animation->SetFlip(false);
                grabbing_wall_right = true;
            }
        }
        else{
            grabbing_wall = false;
            grabbing_wall_right = false;
            grabbing_wall_left = false;
        }


    }
    else{ // is grounded
        cyote_time_buffer_tracked = cyote_time_buffer;
    }

    if(grab_delay_tracked > 0){
        grab_delay_tracked -= Time::Dt();
    }


    if(jump_buffer_tracked > 0){
        SuggestJump();
    }

    if(jump_buffer_tracked >= 0){
        jump_buffer_tracked -= Time::Dt();
    }
    if(cyote_time_buffer_tracked >= 0){
        cyote_time_buffer_tracked -= Time::Dt();
    }
    if(cyote_time_left_wall_tracked >= 0){
        cyote_time_left_wall_tracked -= Time::Dt();
    }
    if(cyote_time_right_wall_tracked >= 0){
        cyote_time_right_wall_tracked -= Time::Dt();
    }
    if(wall_jump_control_blend_tracked > 0){
        wall_jump_control_blend_tracked -= Time::Dt();
        wall_jump_control_blend_tracked = Calc::Clamp(wall_jump_control_blend_tracked, 0, 9999);
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

                // hold onto jump
                jump_buffer_tracked = jump_buffer;
                SuggestJump();

                break;
            }       
        }
        
    }       
}

void PlayerController::SuggestJump(){
    if(cyote_time_buffer_tracked > 0){
        Jump();
        jump_buffer_tracked = 0;
    }
    else if(cyote_time_left_wall_tracked > 0){
        LeftWallJump();
        jump_buffer_tracked = 0;
    }
    else if(cyote_time_right_wall_tracked > 0){
        RightWallJump();
        jump_buffer_tracked = 0;
    }
}

void PlayerController::Jump(){

    pb->velocity.y = -jump_height;
}

void PlayerController::LeftWallJump(){

    pb->velocity.y = -wall_kickoff_height;
    forced_velocity_x = wall_kickoff_force; 

    wall_jump_control_blend_tracked = wall_jump_control_blend;

    grabbing_wall = false;
    grabbing_wall_left = false;
    grabbing_wall_right = false;
    grab_delay_tracked = grab_delay;
}

void PlayerController::RightWallJump(){

    pb->velocity.y = -wall_kickoff_height;
    forced_velocity_x = -wall_kickoff_force; 

    wall_jump_control_blend_tracked = wall_jump_control_blend;

    
    grabbing_wall = false;
    grabbing_wall_right = false;
    grabbing_wall_left = false;
    grab_delay_tracked = grab_delay;
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