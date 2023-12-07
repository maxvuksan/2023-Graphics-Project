#pragma once
#include "UtilityStation.h"
#include "../GameUI/SlotSet.h"

class Container : public UtilityStation{

    public:

        void OnStart() override;
        void OnOpen() override;
        void OnClose() override;

    private:

        SlotSet* slot_set;

};

