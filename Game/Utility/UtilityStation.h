#pragma once
#include "../../Amber/Framework.h"
#include "../Items/ItemDictionary.h"
#include "../World/Chunk.h"
#include "../GameUI/SlotSet.h"

/*
    the base for all interactable objects. interactables are defined by there coordinate and footprint (how many tiles they occupy from said coordinate)
*/
class Chunk;
class UtilityStation : public Object {

    public:

        void Start() override;

        void LinkChunk(Chunk* chunk);
        virtual void OnStart(){}

        // sets the type of station, only utility item codes should be provided
        void SetItemType(ItemCode item);
        
        void Update() override;

        void CatchEvent(sf::Event event) override;

        // called when the station interaction begins (i.e we click station)
        virtual void OnOpen(){}
    
        // called when the station interaction has ended
        virtual void OnClose(){}

        // determines if the utility station can be broken, is decided by its breaking_behaviour (in utility_data)
        bool CanBreak();
        
        /*
            @returns true if the bounds of this station overlaps the provided footprint
            @param other_position the world position snapped to the tile grid locations
            @param other_footprint the dimensions of the utility object we are checking against     
        */
        bool FootprintOver(sf::Vector2i other_position, sf::Vector2i other_footprint);

        const UtilityBlockData* GetUtilityData(){return utility_data;}

    protected:
        Chunk* chunk;
        const UtilityBlockData* utility_data;
        SlotSet* slot_set;

    private:
        bool hovered = false;


};