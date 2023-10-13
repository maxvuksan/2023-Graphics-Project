#pragma once
#include "../Amber/Framework.h"
#include "BlockTypes.h"
#include "CursorGraphic.h"

class World;
class Player : public Object{

    public:

        void Start() override;
        void Update() override;
        void CatchEvent(sf::Event) override;
        void Draw(sf::RenderTarget&) override;

        void CalculateMouse();


        void LinkWorld(World* world){this->world = world;}

    private:

        CursorGraphic* cursor_graphic;
        sf::Vector2i focused_block_position;
        float breaking_completeness;
        float sound_increment;
        int focused_block;

        World* world;
        // where in world space is the mouse hovering over
        sf::Vector2i mouse_pos;
        sf::RectangleShape cursor_shape;

        int selected_block;

        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;

        PhysicsBody* pb;
        float jump_height = 0.15;

};