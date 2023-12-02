#include "Player.h"

void Player::Start(){

    auto sr = AddComponent<SpriteRenderer>();
    sr->SetTexture("player", true);
    sr->SetRenderLayer(2);
}

