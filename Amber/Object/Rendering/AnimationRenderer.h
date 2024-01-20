#pragma once
#include "AnimationSet.h"
#include "../Component.h"

class AnimationRenderer : public Component{
    
    public:

        AnimationRenderer();

        void Start() override;

        void Update() override;

        void Draw(sf::RenderTarget& surface) override;

        sf::Sprite& GetSprite(){return sprite;}

        void SetAnimationSet(const char* label);
        void SetState(const char* label);

        void SetFlip(bool state);
        bool GetFlip();


    private:


        int index = 0;
        float timer = 0;
        
        int flip_factor;

        const char* current_state;

        sf::Sprite sprite;
        AnimationSet* set;
        Animation* current_animation;
};