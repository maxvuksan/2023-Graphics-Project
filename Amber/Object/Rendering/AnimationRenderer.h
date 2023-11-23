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

    private:

        int index = 0;
        float timer = 0;


        sf::Sprite sprite;
        AnimationSet* set;
        Animation* current_animation;
};