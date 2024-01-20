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
        
        void SetLastLeftCollision(float);
        void SetLastRightCollision(float);
        void SetLastBottomCollision(float);
        void SetLastTopCollision(float);

        float GetLastLeftCollision() const {return last_left_collision;}
        float GetLastRightCollision() const {return last_right_collision;}
        float GetLastBottomCollision() const {return last_top_collision;}
        float GetLastTopCollision() const {return last_bottom_collision;}
    private:

        // the time since a collision was made
        float last_left_collision;
        float last_right_collision;
        float last_top_collision;
        float last_bottom_collision;

        bool gravity_on = true;
        static float max_movement;
        static int max_number_of_steps;

        float gravity_factor = 0.0005;
        float gravity_max = 1;

};