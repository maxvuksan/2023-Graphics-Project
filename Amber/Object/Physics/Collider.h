#pragma once
#include "../Component.h"
#include "CollisionInteractionMode.h"

class PhysicsBody;
class Collider : public Component{

    /*
        Collider:

        Base class for all collision objects, 
        has virtual method ResolveCollision() which will be called against each collider

        preventing large movements skipping colliders...
        https://gamedev.stackexchange.com/questions/147502/in-what-order-should-collisions-be-resolved
        
    */

    public:
        virtual void Move(sf::Vector2f movement, PhysicsBody* pb){}

        // is the collider a trigger?
        bool IsTrigger() const {return is_trigger;}
        void SetIsTrigger(bool state){ is_trigger = state;}

        void SetTriggered(bool state){ triggered = state;}
        // is something overlapping the trigger
        bool Triggered() const{ return triggered;}

        void SetCollisionInteractionMode(CollisionInteractionMode mode){collision_interaction_mode = mode;}
        CollisionInteractionMode GetCollisionInteractionMode() const{return collision_interaction_mode;}

    protected:
        CollisionInteractionMode collision_interaction_mode;

    private:
        bool is_trigger = false;
        bool triggered = false;

};