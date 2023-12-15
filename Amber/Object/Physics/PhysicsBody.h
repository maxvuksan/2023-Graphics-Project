#pragma once
#include "../Component.h"

class PhysicsBody : public Component{

    /*
        PhysicsBody

        controlling movement while acknowledging the Colliders in the scene
    */

    public:
        
        PhysicsBody();

        void Update() override;
        // moves the objects Transform, resolving collisions accordingly
        void Move(sf::Vector2f movement);
        void SetGravityState(bool state);

        sf::Vector2f velocity;
        float gravity;
        

    private:
        bool gravity_on = true;
        static float max_movement;
        static int max_number_of_steps;

        float gravity_factor = 0.0005;
        float gravity_max = 1;

};