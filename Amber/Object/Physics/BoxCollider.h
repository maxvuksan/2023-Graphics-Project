#pragma once
#include "Collider.h"
#include <iostream>
#include "../Object.h"



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

            void LinkTransform(Transform* transform){
                this->transform = transform;
            }

            float Left(){
                return x + transform->position.x;
            }
            float Right(){
                return x + (int)width + transform->position.x;
            }
            float Top(){
                return y + transform->position.y;
            }
            float Bottom(){
                return y + (int)height + transform->position.y;
            }

        };


        void Start() override;
        void Update() override;
        void OnDestroy() override;

        void Move(sf::Vector2f movement, PhysicsBody* pb) override;
        std::vector<Rect*> DetermineBoxOverlaps();


        void Draw_Debug(sf::RenderTarget& surface) override;

        void SetOffset(sf::Vector2f offset, int index = 0);
        void SetSize(sf::Vector2f size, int index = 0);

        /*
            For colliders with PhysicBodies we should only ever have one rectangle
        */
        void AddRect(int x, int y, unsigned int width, unsigned int height);
        void AddRect(Rect rect);

        std::vector<Rect>& GetRects();  
        void ClearRects();

    private:
        std::vector<Rect> rects;     
};