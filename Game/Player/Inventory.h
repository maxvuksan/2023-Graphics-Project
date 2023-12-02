#pragma once
#include "../../Amber/Framework.h"
#include "../GameUI/SlotSet.h"

class Inventory : public Object {

    friend class Slot;

    public:

        void Start() override;
        void Draw(sf::RenderTarget& surface) override;
        
        void CatchEvent(sf::Event event);
        void LeftClickOnSlot();
        void RightClickOnSlot();

        void SetSelectedSlotSprite();

        void PickupItem(ItemCode item);
        // returns an ItemCode if the slot is occupued, otherwise returns -1
        int GetItemInSelectedSlot();
        // decrements a selected inventory slots count, if count is 0, mark as not occupied
        void DecrementSelectedSlot();

        Slot* GetSlot(int x, int y);
        Slot* GetSlot(sf::Vector2i coordinate);

    private:

        // @returns the x, y coordinate of the first slot containing the given item, returns -1, -1 if not found
        sf::Vector2i FindSlotContainingItem(ItemCode item);
        // @returns the x, y coordinate of the first free slot, -1, -1 if no free slots
        sf::Vector2i FindNextFreeSlot();
        void AddItemToSlot(ItemCode item, int slot_x, int slot_y);

        SlotSet* hotbar_slot_set;
        SlotSet* backpack_slot_set;

        static int row_length;
        static int row_count;
        static sf::Color ui_overlay_colour;

        sf::RectangleShape rectangle_shape;
        sf::RectangleShape rectangle_shape_inside;

        sf::Vector2i hovered_slot;
        bool holding_item; // we have lifted an item out of a slot
        ItemCode held_item;
        unsigned short held_item_count;
        sf::Sprite held_item_sprite;

        int selected_slot;

        bool open;
};