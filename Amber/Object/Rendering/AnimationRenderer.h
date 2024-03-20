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

        // rather than setting by state name we can set directly by the states index in its array
        void SetStateByIndex(int state_index);

        const char* GetState(){return current_state;}
        int GetStateIndex(){return state_index;}

        void SetFlip(bool state);
        bool GetFlip();

        void SetUseFixedPosition(bool state){use_set_position = state;}
        bool GetUseFixedPosition(){return use_set_position;}

        void SetFixedPosition(sf::Vector2f position){fixed_position = position;}
        sf::Vector2f GetFixedPosition(){return fixed_position;}

    private:

        bool use_set_position;
        sf::Vector2f fixed_position;

        int index = 0;
        float timer = 0;
        
        int flip_factor;

        int state_index = 0;
        const char* current_state;

        sf::Sprite sprite;
        AnimationSet* set;
        Animation* current_animation;
};