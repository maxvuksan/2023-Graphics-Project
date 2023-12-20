#pragma once
#include "../../Amber/Framework.h"
#include "../Items/ItemDictionary.h"

class Player : public Object{

    public:

        void Start() override;
        void Update();

        ItemCode item_in_hand;

};