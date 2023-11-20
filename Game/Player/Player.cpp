#include "Player.h"

void Player::Start(){

    AddComponent<SpriteRenderer>()->SetTexture("player", true);
}

