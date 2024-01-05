#pragma once
#include "UtilityStation.h"

class Container : public UtilityStation{

    public:

        void OnOpen() override;
        void OnClose() override;

};

