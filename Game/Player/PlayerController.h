#pragma once
#include "../../Amber/Framework.h"

class World;
class PlayerController : public Component {

    public:
        
        void Start() override;

        void UpdateEventFocusBounded() override;
        void CatchEvent(sf::Event event) override;

        void SuggestJump();
        void Jump();
        void LeftWallJump();
        void RightWallJump();

        void Respawn();

        void SetFlyMode(bool state);
        bool GetFlyMode();

    private:

        bool in_fly_mode;

        bool grabbing_wall;
        bool grabbing_wall_left;
        bool grabbing_wall_right;
    
        SpriteRenderer* sr;
        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;
        BoxCollider* body_collider;
        AnimationRenderer* animation;
        PhysicsBody* pb;
        
        float jump_buffer_tracked;
        float cyote_time_buffer_tracked;
        float cyote_time_left_wall_tracked;
        float cyote_time_right_wall_tracked;
        float grab_delay_tracked;
        float wall_jump_control_blend_tracked;

        // blend between 
        float controlled_velocity_x; 
        float forced_velocity_x;

        static float speed;
        static float climb_speed;
        static float jump_buffer;
        static float cyote_time_buffer;
        static float cyote_time_wall;
        static float jump_height;
        static float grab_delay;
        static float wall_kickoff_force;
        static float wall_jump_control_blend; 
        static float wall_kickoff_height;
};