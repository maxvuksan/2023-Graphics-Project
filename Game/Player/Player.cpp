#include "Player.h"
#include "LightSource.h"

void Player::Start(){

    auto sr = AddComponent<SpriteRenderer>();
    sr->SetTexture("player", true);
    sr->SetRenderLayer(2);
    
    AddComponent<LightSource>();
}

