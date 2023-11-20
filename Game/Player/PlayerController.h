#pragma once
#include "../../Amber/Framework.h"

class World;
class PlayerController : public Component {

    public:
        
        void Start() override;
        void UpdateEventFocusBounded() override;
        void CatchEvent(sf::Event event) override;

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


        PhysicsBody* pb;
        float jump_height = 0.15;
};