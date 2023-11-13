#include "PhysicsBody.h"
#include "../Object.h"
#include "Collider.h"
#include <cmath>

float PhysicsBody::max_movement = 7;

PhysicsBody::PhysicsBody() : velocity(0,0), gravity_on(true), gravity(1){}

void PhysicsBody::Update(){

    if(gravity_on){
        if(velocity.y < gravity_max){
            velocity.y += gravity * gravity_factor * Time::Dt();
        }
        else{
            velocity.y = gravity_max;
        }
    }

    Move(velocity * Time::Dt());
}

void PhysicsBody::Move(sf::Vector2f movement){

    std::vector<Collider*> colliders = object->GetAllComponentsOf<Collider>();

    for(auto col : colliders){
        // finds first collider which is NOT a trigger...
        
            if(!col->IsTrigger()){
                /* attempting step based movement 
                if(false){//movement.x > max_movement || movement.y > max_movement){
                    float subtract_x = 1;
                    float subtract_y = 1;

                    if(movement.x != 0){
                        subtract_x = movement.x / abs(movement.x);
                    }
                    else if(movement.y != 0){
                        subtract_y = movement.y / abs(movement.y);
                    }
    
                    // preventing large movements from skipping colliders
                    while(abs(movement.x) > 0 || abs(movement.y) > 0){

                        float x_mov = movement.x;
                        if(abs(movement.x) > max_movement){
                            x_mov = max_movement * subtract_x;
                        }

                        float y_mov = movement.y;
                        if(abs(movement.y) > max_movement){
                            y_mov = max_movement * subtract_y;
                        }

                        col->Move(sf::Vector2f(x_mov, y_mov), this);

                        movement.x -= max_movement * subtract_x;
                        movement.y -= max_movement * subtract_y;

                        if(movement.x * subtract_x < 0){
                            movement.x = 0;
                        }
                        if(movement.y * subtract_y < 0){
                            movement.y = 0;
                        }
                    }
                }*/
                
                col->Move(movement, this);
                return;
            
            }

    }
    // no solid colliders...
    object->GetTransform()->position += movement;
}

void PhysicsBody::SetGravityState(bool state){
    gravity_on = state;
}
