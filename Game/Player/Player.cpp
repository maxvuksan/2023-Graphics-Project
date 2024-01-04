#include "Player.h"
#include "../LightSource.h"
#include "../Items/ItemDictionary.h"
#include "../World/LightingManager.h"

void Player::Start(){

    auto ar = AddComponent<AnimationRenderer>();

    ar->SetAnimationSet("player");
    ar->SetRenderLayer(2);
    
    auto light = AddComponent<LightSource>();
    light->decay = 0.03;

    AddComponent<PixelParticleSource>();

}

void Player::Update(){

    if(item_in_hand == ItemCode::item_Torch){
        
        // TO DO : Add player visual component, allows player components to talk to each other and is responsible for players appearnace and item in hand
        
        //LightingManager::PropogateLighting(,ItemDictionary::torch_colour, ItemDictionary::torch_propogate_decay);
    }
}
