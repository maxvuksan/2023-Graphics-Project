#pragma once
#include "../../Amber/Framework.h"
#include "Slot.h"
#include "../Items/Recipes.h"

class SlotSet;

struct HoveredSlot{
    SlotSet* set_parent; // where slot is from
    int x; // x coord
    int y; // y coord
};

class SlotSet : public Object{
 
    public:

        static void Construct();

        // should be called after a grid is defined
        void Align(ScreenLocationX align_x, ScreenLocationY align_y);

        void DefineRecipeGrid(const std::vector<Recipes>& recipes);
        void DefineGrid(int width, int height, SlotType type);
        void SetSlotType(int slot_x, int slot_y, SlotType type);

        int GetWidth();
        int GetHeight();

        int GetRowLength();
        int GetRowCount();

        void Draw(sf::RenderTarget& surface) override;

        // @returns true if all slots are empty (count of 0), false otherwise
        bool IsEmpty();

        // @returns a struct containing what SlotSet, and what coordinate within said slot set the mouse is over, nullptr and -1 coords if nothing is hovered
        static HoveredSlot GetHoveredSlotFromMultipleSets(std::vector<SlotSet*>& slot_sets);
        std::vector<std::vector<Slot>>* GetSlots() { return &slots; }

        Slot* GetSlot(int x, int y){ return &slots.at(x).at(y); }
        
        const RecipeData& GetRecipe(int x, int y);
    private:

        std::vector<std::vector<Recipes>> recipes;
        std::vector<std::vector<Slot>> slots;

        sf::Vector2i hovered_slot;

        static sf::Sprite slot_sprite;
};  