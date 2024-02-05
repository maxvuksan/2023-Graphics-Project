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
        void ShiftClickOnSlot();
        void MiddleClickOnSlot();
        void ClickOnRecipeSlot(bool move_to_holding = true);

        // @returns true if a recipes ingredients are found in the players hotbar and/or backpack 
        bool HasIngredientsForRecipe(const RecipeData& recipe);
        /*
            @param item the ItemCode we are looking for
            @param slot_set the SlotSet we are searching through
            @returns the total count of a specific ItemCode in the provided slot_set
        */
        int CountItemInSlotSet(SlotSet* slot_set, ItemCode item);
        // removes a recipes ingredients from the players inventory, should be called after ensuring they after present (through HasIngredientsForRecipe())
        void RemoveIngredientsForRecipe(const RecipeData& recipe);
        void RemoveItemFromSlotSet(SlotSet* slot_set, ItemCode item_code, int* count_remaining);

        void SetSelectedSlotSprite();

        void PickupItem(ItemCode item, int count = 1);
        
        // @returns the ItemCode present in the selected slot, otherwise returns -1 if the selected slot is empty
        int GetItemInSelectedSlot();
        
        void DecrementSelectedSlot();

        /*
            @returns a pointer to the slot hovered by the mouse
        */
        Slot* GetHoveredSlot();


        // width
        static int GetRowLength(){return row_length;}
        // height
        static int GetRowCount(){return row_count;}

        Slot* GetSlot(int x, int y);


    private:

        /*
            @param slot_set the slot set we are searching through
            @param item the ItemCode of said item
            
            @returns the x, y coordinate of the first slot containing the given item, returns -1, -1 if not found
        */
        sf::Vector2i FindSlotContainingItem(SlotSet* slot_set, ItemCode item, bool ignore_full = true);
        /*
            @param slot_set the SlotSet we are searching through
            @returns the x, y coordinate of the first free slot, -1, -1 if no free slots
        */
        sf::Vector2i FindNextFreeSlot(SlotSet* slot_set);

        /*
            @param slot_set the slot set we are searching through
            @param item the ItemCode of said item
            @param slot_x the x coordinate of the slot within the SlotSet
            @param slot_y the y coordinate of the slot within the SlotSet
            @param count_remaining a pointer to a item count, is decremented by the function allowing the caller to determine how many items could not be fit into said slot
        
            @returns true if any amount of provided item could be added to the slot, false if the slot is full
        */    
        bool AddItemToSlot(SlotSet*, ItemCode item, int slot_x, int slot_y, int* count_remaining);

        SlotSet* hotbar_slot_set;
        SlotSet* backpack_slot_set;
        SlotSet* inventory_crafting_slot_set;
        

        static int row_length;
        static int row_count;

        sf::RectangleShape rectangle_shape;
        sf::RectangleShape rectangle_shape_inside;

        sf::Vector2i hovered_slot;
        
        bool holding_item; // we have lifted an item out of a slot
        ItemCode held_item;
        unsigned short held_item_count;
        sf::Text held_item_text;
        sf::Sprite held_item_sprite;

        int selected_slot;
};