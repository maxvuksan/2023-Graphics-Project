#pragma once
#include "../Component.h"
#include <unordered_map>
#include "Animation.h"
/*
    A group of animations, stored in states (labels)
*/
class AnimationSet {

    public:
        void Init(std::vector<Animation> animations, std::vector<const char*> labels);

        // @returns a reference to animation at label, nullptr if no animation exists
        Animation* GetState(const char* label);
        int GetStateIndex(const char* label);
        const char* GetStateLabelByIndex(int state_index);
        Animation* GetStateByIndex(int state_index);
        
        Animation* GetDefaultState();
        int GetDefaultStateIndex();

    private:
        std::unordered_map<const char*, Animation> states; 
        const char* default_state;
};