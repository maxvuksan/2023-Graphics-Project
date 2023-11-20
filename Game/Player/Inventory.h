#pragma once
#include "../../Amber/Framework.h"
#include "InventorySlot.h"

class Inventory : public Object {

    friend class InventorySlot;

    public:

        void Start() override;
        void Draw(sf::RenderTarget& surface) override;
        void CatchEvent(sf::Event event);
    

        void PickupItem(ItemCode item);

        // returns an ItemCode if the slot is occupued, otherwise returns -1
        int GetItemInSelectedSlot();
        // decrements a selected inventory slots count, if count is 0, mark as not occupied
        void DecrementSelectedSlot();

    private:

        // @returns the x, y coordinate of the first slot containing the given item, returns -1, -1 if not found
        sf::Vector2i FindSlotContainingItem(ItemCode item);
        // @returns the x, y coordinate of the first free slot, -1, -1 if no free slots
        sf::Vector2i FindNextFreeSlot();
        void AddItemToSlot(ItemCode item, int slot_x, int slot_y);


        sf::RectangleShape rectangle_shape;
        sf::RectangleShape rectangle_shape_inside;

        std::vector<std::vector<InventorySlot>> slots;
        int selected_slot;

        bool open;

        static int row_count; 
        static int row_length;
};