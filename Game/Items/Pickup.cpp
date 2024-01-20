#include "Pickup.h"

float Pickup::attract_speed = 400;
float Pickup::popin_speed = 300;

Pickup::Pickup() : target(nullptr), attract_tracked(0){}

void Pickup::Start(){

    SetRenderLayer(3);
    GetTransform()->scale = sf::Vector2f(0.5f,0.5f);
    sr = AddComponent<SpriteRenderer>();
    sr->SetOffset(sf::Vector2f(1,1));
    popin_speed_tracked = 0;
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


    popin_speed_tracked += Time::Dt();

    float lerped = Calc::EaseOutBack(popin_speed_tracked / popin_speed);
    float lerp_clamp = Calc::Clamp(lerped, 0, 1);
    GetTransform()->scale = sf::Vector2f(lerp_clamp * 0.5f, lerp_clamp * 0.5f);


    if(lerped < 2.5){

        return;
    }
    

    if(target != nullptr){

        attract_tracked += Time::Dt();

        // add pickup to inventory, then destory
        if(attract_tracked > attract_speed){
            Destroy();
        }

        GetTransform()->position = Calc::Lerp(GetTransform()->position, target->position, attract_tracked / attract_speed);
    }

}
