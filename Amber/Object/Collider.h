#pragma once
#include "Component.h"

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
        bool IsTrigger(){return is_trigger;}
        void SetIsTrigger(bool state){ is_trigger = state;}

        void SetTriggered(bool state){ triggered = state;}
        // is something overlapping the trigger
        bool Triggered(){ return triggered;}
    
    private:
        bool is_trigger = false;
        bool triggered = false;

};