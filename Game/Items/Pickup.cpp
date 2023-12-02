#include "Pickup.h"

float Pickup::attract_speed = 400;

Pickup::Pickup() : target(nullptr), attract_tracked(0){}

void Pickup::Start(){

    SetRenderLayer(3);
    GetTransform()->scale = sf::Vector2f(0.5f,0.5f);
    sr = AddComponent<SpriteRenderer>();
    sr->SetOffset(sf::Vector2f(1,1));
}


void Pickup::SetItemCode(ItemCode item){
    this->item_code = item;
    ItemDictionary::SetItemSprite(*sr->GetSprite(), item);
}
ItemCode Pickup::GetItemCode(){
    return item_code;
}

/*
    note when a player is destroyed, the attraction may cause seg fault
*/
void Pickup::AttractToTransform(Transform* object){
    this->target = object;
    on_attract_position = GetTransform()->position;
}

void Pickup::Update(){
    if(target != nullptr){

        attract_tracked += Time::Dt();

        // add pickup to inventory, then destory
        if(attract_tracked > attract_speed){
            Destroy();
        }

        GetTransform()->position = Calc::Lerp(GetTransform()->position, target->position, attract_tracked / attract_speed);
    }
}
