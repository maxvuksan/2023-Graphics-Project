#include "PhysicsBody.h"
#include "Object.h"
#include "Collider.h"

void PhysicsBody::Update(){

    if(velocity.y < gravity_max){
        velocity.y += gravity * gravity_factor * Time::Dt();
    }
    else{
        velocity.y = gravity_max;
    }

    Move(velocity * Time::Dt());
}

void PhysicsBody::Move(sf::Vector2f movement){

    std::vector<Collider*> colliders = object->GetAllComponentsOf<Collider>();

    for(auto col : colliders){
        // finds first collider which is NOT a trigger...
        
        if(!col->IsTrigger()){
            col->Move(movement, this);
            return;
        }

    }
    // no solid colliders...
    object->GetTransform()->position += movement;
}