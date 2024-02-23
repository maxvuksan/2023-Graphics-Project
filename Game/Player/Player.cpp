#include "Player.h"
#include "../World/Lighting/LightSource.h"
#include "../Items/ItemDictionary.h"
#include "../World/Lighting/LightingManager.h"

void Player::Start(){

    AddComponent<PhysicsBody>();
    auto ar = AddComponent<AnimationRenderer>();
    ar->SetAnimationSet("player");
    ar->SetRenderLayer(8);
    
    auto body_collider = AddComponent<BoxCollider>();
    body_collider->SetSize(sf::Vector2f(8, 16));
    body_collider->SetOffset(sf::Vector2f(-4,-8));
    
    AddComponent<PixelParticleSource>();

}

void Player::Update(){

    if(item_in_hand == ItemCode::item_Torch){
        
        // TO DO : Add player visual component, allows player components to talk to each other and is responsible for players appearnace and item in hand
        
        //LightingManager::PropogateLighting(,ItemDictionary::torch_colour, ItemDictionary::torch_propogate_decay);
    }
}
