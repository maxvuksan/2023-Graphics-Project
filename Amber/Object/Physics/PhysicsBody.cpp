#include "PhysicsBody.h"
#include "../Object.h"
#include "Collider.h"
#include <cmath>
#include "../../Utility/Calc.h"

float PhysicsBody::max_movement = 7;
int PhysicsBody::max_number_of_steps = 100; // allowed number of max_movement sub steps, ignores any after that point

PhysicsBody::PhysicsBody() : velocity(0,0), gravity_on(true), gravity(1), last_left_collision(1000), last_right_collision(1000), last_bottom_collision(1000), last_top_collision(1000){}

void PhysicsBody::Update(){

    if(gravity_on){
        if(velocity.y < gravity_max){
            velocity.y += gravity * gravity_factor * Time::Dt();
        }
        else{
            velocity.y = gravity_max;
        }
    }

    // prevent potentially float overflow if a collision does not occur for a LONG time
    if(last_left_collision < 100000){
        last_left_collision += Time::Dt();
    }
    if(last_right_collision < 100000){
        last_right_collision += Time::Dt();
    }
    if(last_top_collision < 100000){
        last_top_collision += Time::Dt();
    }
    if(last_bottom_collision < 100000){
        last_bottom_collision += Time::Dt();
    }

    Move(velocity * Time::Dt());
}

void PhysicsBody::Move(sf::Vector2f movement){

    std::vector<Collider*> colliders = object->GetAllComponentsOf<Collider>();

    for(auto col : colliders){
        // finds first collider which is NOT a trigger...
        
            if(!col->IsTrigger()){

                // move in increments to prevent low FPS physics problems (e.g. skipping colliders)
                if(movement.x > max_movement || movement.y > max_movement){

                    float angle = Calc::VectorToRadians(movement);
                    sf::Vector2f norm_vector = Calc::RadiansToVector(angle);
                    sf::Vector2f step_vector = sf::Vector2f(norm_vector.x * max_movement, norm_vector.y * max_movement);

                    sf::Vector2f distance_so_far(0,0);
 
                    for(int i = 0; i < max_number_of_steps; i++){

                        col->Move(step_vector, this);
                        
                        distance_so_far += step_vector;

                        if(abs(distance_so_far.x) >= abs(movement.x) || abs(distance_so_far.y) >= abs(movement.y)){
                            break;
                        }
                    }
                    return;
                }
                else{ // move in full
                    col->Move(movement, this);
                    return;
                }
            
            }

    }
    // no solid colliders...
    object->GetTransform()->position += movement;
}

void PhysicsBody::SetGravityState(bool state){
    gravity_on = state;
}

void PhysicsBody::SetLastLeftCollision(float value){
    last_left_collision = value;
}
void PhysicsBody::SetLastRightCollision(float value){
    last_right_collision = value;
}
void PhysicsBody::SetLastBottomCollision(float value){
    last_bottom_collision = value;
}
void PhysicsBody::SetLastTopCollision(float value){
    last_top_collision = value;
}