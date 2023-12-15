#pragma once
#include "../../Amber/Framework.h"

class World;
class PlayerController : public Component {

    public:
        
        void Start() override;

        void UpdateEventFocusBounded() override;
        void CatchEvent(sf::Event event) override;

        void Jump();

        void Respawn();

        void SetFlyMode(bool state);
        bool GetFlyMode();

    private:

        bool in_fly_mode;
    
        SpriteRenderer* sr;
        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;
        BoxCollider* body_collider;

        AnimationRenderer* animation;


        PhysicsBody* pb;

        
        float jump_buffer_tracked;
        float cyote_time_buffer_tracked;
        
        static float jump_buffer;
        static float cyote_time_buffer;

        static float jump_height;
};