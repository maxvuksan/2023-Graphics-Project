#pragma once
#include "Collider.h"
#include <iostream>
#include "../Object.h"
#include "ColliderType.h"


class BoxCollider : public Collider {

    /*
        BoxCollider

        A rectangle shaped collider
    */
    public:
    
        struct Rect{
            int x = 0;
            int y = 0;
            int width = 16;
            int height = 16;  

            Transform* transform = nullptr;

            ColliderType collision_mode = ColliderType::SOLID;

            void LinkTransform(Transform* transform){
                this->transform = transform;
            }

            float Left() const{
                return x + transform->GetGlobalPosition().x;
            }
            float Right() const{
                return x + width + transform->GetGlobalPosition().x;
            }
            float Top() const{
                return y + transform->GetGlobalPosition().y;
            }
            float Bottom() const{
                return y + height + transform->GetGlobalPosition().y;
            }

        };


        void Start() override;
        void Update() override;
        ~BoxCollider() override;


        /*
            translates the collider by the movement vector, should be called by PhysicsBody
        */
        void Move(sf::Vector2f movement, PhysicsBody* pb) override;
        void Move(sf::Vector2f movement, PhysicsBody* pb, int move_depth);

        /*
            attempts to step this collider up a provided rect

            @returns true if the step was successful, false otherwise
        
            @attention this function will only consider the collision rectangle with index 0 (is meant for single rectangular colliders)
        */
        bool StepUpCollider(float movement_x, Rect* other_rect, PhysicsBody* pb);

        /*
            snaps the collider within the scene bounds
            @attention this function will only consider the collision rectangle with index 0 (is meant for single rectangular colliders)
        */
        void ContainToSceneBounds();
        
        /*
            @returns a vector of all colliders in the scene which are currently overlapping this one,

            @attention this function will only consider the collision rectangle with index 0 (is meant for single rectangular colliders)
            @param previous_frame_bottom the position of the bottom edge of the collision rectangle last frame
        */
        std::vector<Rect*> DetermineBoxOverlaps(float previous_frame_bottom);

        /*
            determines if a rectangle of the provided properties would overlay this collider
        */
        bool Overlaps(float other_left, float other_right, float other_top, float other_bottom);

        /*
            draws debug visuals for the component 
            @param surface location to draw to
        */
        void DrawDebug(sf::RenderTarget& surface) override;

        /*
            sets the offset of a collision rectangle

            @param offset x and y offset of the rectangle
            @param index the index of said collision rect (for single rectangle colliders this is left at 0)
        */
        void SetOffset(sf::Vector2f offset, int index = 0);

        /*
            sets the dimensions of a collision rectangle

            @param size the width * height of the rectangle
            @param index the index of said collision rect (for single rectangle colliders this is left at 0)
        */
        void SetSize(sf::Vector2f size, int index = 0);

        /*
            adds a new collision rectangle to the collider 

            @attention For colliders with PhysicBodies we should only ever have one rectangle

            @param x offset of the rectangle
            @param y offset of the rectangle
            @param width dimension of the rectangle
            @param height dimension of the rectangle
            @param collision_mode how the collider behaves
        */
        void AddRect(int x, int y, unsigned int width, unsigned int height, ColliderType collision_mode);
        
        /*
            adds a new collision rectangle to the collider 

            @attention For colliders with PhysicBodies we should only ever have one rectangle
            @param rect new rectangle to copy
        */
        void AddRect(Rect rect);

        // @returns a reference to all collision rectangles
        std::vector<Rect>& GetRects();  
        // clears all collision rectangles
        void ClearRects();

        /*
            determines how high the collider is allowed to step when pushed agaisnt a wall
            @param step_height the maximum allowed vertical translation
        */
        void SetStepHeight(float step_height) {this->step_height = step_height;}
        /*
            @returns the maximum step height of the collider
        */
        float GetStepHeight(){ return step_height;}

    private:

        float step_height = 0;
        std::vector<Rect> rects;     
};