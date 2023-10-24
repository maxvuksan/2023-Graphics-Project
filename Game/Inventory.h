#pragma once
#include "../Amber/Framework.h"
#include "InventorySlot.h"

class Inventory : public Object {

    friend class InventorySlot;

    public:

        void Start() override;

    private:

        
        std::vector<std::vector<InventorySlot>> slots;
        
        const int cellsize = 12;

        const int row_count = 8;
        const int row_length = 5;

        const sf::Color ui_colour = sf::Color(0,0,0,0.5);
};