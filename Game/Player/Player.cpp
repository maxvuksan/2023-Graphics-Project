#include "Player.h"
#include "../LightSource.h"

void Player::Start(){

    auto ar = AddComponent<AnimationRenderer>();

    ar->SetAnimationSet("player");
    ar->SetRenderLayer(2);
    
    AddComponent<LightSource>();

}

