#pragma once
#include "../../Amber/Framework.h"

class World;
class HealthBar;
class PlayerController : public Component {

    public:
        
        void Start() override;
        void LinkHealthBar(HealthBar* health_bar);
        void LinkWorld(World* world);

        void UpdateEventFocusBounded() override;
        void CatchEvent(sf::Event event) override;

        void SuggestJump();
        void Jump();
        void LeftWallJump();
        void RightWallJump();

        void Die();
        void Respawn();

        void SetFlyMode(bool state);
        bool GetFlyMode();

        void TakeDamage(int damage);

    private:
    
        void ApplyFallDamage();
        void SetHeightWhenGrounded();

        HealthBar* health_bar;
        World* world;

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

        // blend between on wall jumps
        float controlled_velocity_x; 
        float forced_velocity_x;

        static int fall_damage_free_threshold; // how many blocks can the player fall before fall damage starts being applied?
        static float fall_damage_velocity_threshold; // what speed does fall damage begin
        float height_when_grounded;

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