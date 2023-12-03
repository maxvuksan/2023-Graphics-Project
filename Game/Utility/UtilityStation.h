#pragma once
#include "../../Amber/Framework.h"
#include "../Items/ItemDictionary.h"
#include "../World/Chunk.h"

/*
    the base for all interactable objects. interactables are defined by there coordinate and footprint (how many tiles they occupy from said coordinate)
*/
class UtilityStation : public Object {

    public:

        void Start() override;

        void LinkChunk(Chunk* chunk);
        virtual void OnStart(){}

        void SetItemType(ItemCode item);
        
        void Update() override;

        void CatchEvent(sf::Event event) override;

        virtual void OnOpen(){}
        virtual void OnClose(){}
        
        // @returns true if the footprint over laps this stations footprint
        bool FootprintOver(sf::Vector2i position, sf::Vector2i footprint);

        
        
        

    protected:
        Chunk* chunk;
        const UtilityBlockData* utility_data;


    private:
        bool hovered = false;


};