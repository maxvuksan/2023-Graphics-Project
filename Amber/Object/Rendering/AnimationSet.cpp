#include "AnimationSet.h"
#include <iostream>
#include <string.h>

void AnimationSet::Init(std::vector<Animation> animations, std::vector<const char*> labels){

    if(animations.size() != labels.size()){
        std::cout << "ERROR : animation vector and labels vector have different sizes, they must be 1:1, AnimationSet::AnimationSet()\n";
        return;
    }

    for(int i = 0; i < animations.size(); i++){
        states.insert(std::make_pair(labels[i], animations[i]));
        //std::cout << "insert " << labels[i] << "\n";
    }
    default_state = labels[0];
}

Animation* AnimationSet::GetState(const char* label){
    
    for(auto& result : states){

        if(strcmp(result.first, label) == 0){
            return &result.second;
        }
    }

    std::cout << "ERROR : could not find an animation state " << label << ". AnimationSet::GetState()\n";
    return nullptr;

}

int AnimationSet::GetStateIndex(const char* label){
    int i = 0;
    for(auto& result : states){

        if(strcmp(result.first, label) == 0){
            return i;
        }
        i++;
    }

    std::cout << "ERROR : could not find an animation state " << label << ". AnimationSet::GetStateIndex()\n";
    return -1;

}
Animation* AnimationSet::GetStateByIndex(int state_index){

    int i = 0;

    for(auto& result : states){

        if(i == state_index){
            return &result.second;
        }
        i++;
    }

    std::cout << "ERROR : could not find an animation state with index " << state_index << ". AnimationSet::GetStateByIndex()\n";
    return nullptr;
}

const char* AnimationSet::GetStateLabelByIndex(int state_index){

    int i = 0;

    for(auto& result : states){

        if(i == state_index){
            return result.first;
        }
        i++;
    }

    std::cout << "ERROR : could not find an animation state with index " << state_index << ". AnimationSet::GetStateLabelByIndex()\n";
    return nullptr;
}

Animation* AnimationSet::GetDefaultState(){
    return GetState(default_state);
}


int AnimationSet::GetDefaultStateIndex(){
    return GetStateIndex(default_state);
}