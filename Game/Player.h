#pragma once
#include "../Amber/Framework.h"

class World;
class Player : public Object{

    public:

        void Start() override;
        void Update() override;
        void CatchEvent(sf::Event) override;

        void CalculateMouse();


        void LinkWorld(World* world){this->world = world;}

    private:



        World* world;
        // where in world space is the mouse hovering over
        sf::Vector2i mouse_pos;

        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;

        PhysicsBody* pb;
        float jump_height = 0.15;

};