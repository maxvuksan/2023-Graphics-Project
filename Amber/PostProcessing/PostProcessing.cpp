#include "PostProcessing.h"
#include "../Core/AssetManager.h"

std::vector<Effect*> PostProcessing::effect_stack;

void PostProcessing::Push(const char* label){

    effect_stack.push_back(AssetManager::GetPostProcessingEffect(label));
}

void PostProcessing::Clear(){

    effect_stack.clear();
}