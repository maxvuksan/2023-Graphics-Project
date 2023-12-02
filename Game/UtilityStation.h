#pragma once
#include "../Amber/Framework.h"
#include "Items/ItemDictionary.h"

/*
    the base for all interactable objects. interactables are defined by there coordinate and footprint (how many tiles they occupy from said coordinate)
*/
class UtilityStation : public Object {

    public:

        void Start(){
            auto sr = AddComponent<SpriteRenderer>();
        }

        void SetItemType(ItemCode item) {

            if(ItemDictionary::ITEM_DATA[item].type != ItemType::type_Utility){
                std::cout << "ERROR : Attempted to set a UtilityStation to a non utility item index, UtilityStation::SetItemType()\n";
                return;
            }

            utility_data = &ItemDictionary::UTILITY_BLOCK_DATA[ItemDictionary::ITEM_DATA[item].code_in_type];

            ItemDictionary::SetItemSprite(*this->GetComponent<SpriteRenderer>()->GetSprite(), item, false);
        }
        
        // @returns true if a point hovering over the stations footprint
        bool PointOver(sf::Vector2f point){
            if(GetTransform()->position.x + utility_data->footprint.x > point.x && GetTransform()->position.x < point.x){
                if(GetTransform()->position.y + utility_data->footprint.y > point.y && GetTransform()->position.y < point.y){
                    return true;
                }
            }
            return false;
        }

        // @returns true if the footprint over laps this stations footprint
        bool FootprintOver(sf::Vector2i position, sf::Vector2i footprint){

            if(GetTransform()->position.y + utility_data->footprint.y > position.y && GetTransform()->position.y < position.y + footprint.y){
                if(GetTransform()->position.x + utility_data->footprint.x > position.y && GetTransform()->position.x < position.x + footprint.x){
                    return true;
                }
            }

            return false;
        }


    const UtilityBlockData* utility_data;

};