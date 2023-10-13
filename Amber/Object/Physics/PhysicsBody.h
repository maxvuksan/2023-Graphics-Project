#pragma once
#include "../Component.h"

class PhysicsBody : public Component{

    /*
        PhysicsBody

        controlling movement while acknowledging the Colliders in the scene
    */

    public:
        
        void Update() override;
        // moves the objects Transform, resolving collisions accordingly
        void Move(sf::Vector2f movement);
        
        sf::Vector2f velocity;
        float gravity = 1;

    private:
        float gravity_factor = 0.0005;
        float gravity_max = 1;

};