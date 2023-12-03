#pragma once
#include "../../Amber/Framework.h"
#include "Slot.h"

class SlotSet;

struct HoveredSlot{
    SlotSet* set_parent; // where slot is from
    int x; // x coord
    int y; // y coord
};

class SlotSet : public Object{
 
    public:

        static void Construct();
        

        void Start() override;

        void SetAlign(ScreenLocation alignemnt);

        void DefineGrid(int width, int height, SlotType type);
        void SetSlotType(int slot_x, int slot_y, SlotType type);

        void Draw(sf::RenderTarget& surface) override;

        // @returns a struct containing what SlotSet, and what coordinate within said slot set the mouse is over, nullptr and -1 coords if nothing is hovered
        static HoveredSlot GetHoveredSlotFromMultipleSets(std::vector<SlotSet*> slot_sets);
        std::vector<std::vector<Slot>>* GetSlots() { return &slots; }

        Slot* GetSlot(int x, int y){ return &slots.at(x).at(y); }

    private:

        std::vector<std::vector<Slot>> slots;
        UIRect* ui_rect;

        sf::Vector2i hovered_slot;

        static sf::Sprite slot_sprite;
};  