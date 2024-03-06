#pragma once
#include "Collider.h"
#include "BoxCollider.h"

class CircleCollider : public Collider {

    public:
        

        void Start() override;

        void Move(sf::Vector2f movement, PhysicsBody* pb) override;
        // Move recursivley calls itself, to prevent infinite loops limit the allowed recursive depth
        void Move(sf::Vector2f movement, PhysicsBody* pb, int move_depth);
        void ResolveWhenCircleCenterInRect(sf::Vector2f inital_position, const BoxCollider::Rect& rect, PhysicsBody* pb, int move_depth);

        void SetRadius(float radius){this->radius = radius;}
        float GetRadius(){return this->radius;}

        /*
            draws debug visuals for the component 
            @param surface location to draw to
        */
        void DrawDebug(sf::RenderTarget& surface) override;

        std::vector<BoxCollider::Rect*> DetermineBoxOverlaps();


    private:
        float previous_frame_bottom = 0;
        float radius;
        int max_resolutions = 10;
};