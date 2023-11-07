#pragma once
#include "../Amber/Framework.h"
#include "BlockTypes.h"
#include "CursorGraphic.h"

class World;
class PlayerController : public Component {

    public:
        
        void Start() override;
        void Update() override;
        void CatchEvent(sf::Event event) override;

        void LinkWorld(World* world){this->world = world;}
        void CalculateMouse();

    private:
    
        CursorGraphic* cursor_graphic;
        sf::Vector2i focused_block_position;

        float breaking_completeness;
        float sound_increment;
        int focused_block;

        World* world;
        sf::Vector2i mouse_world_pos; // where in world space is the mouse hovering over

        int selected_block;

        SpriteRenderer* sr;
        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;

        PhysicsBody* pb;
        float jump_height = 0.15;
};