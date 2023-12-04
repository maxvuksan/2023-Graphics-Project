#include "SlotSpace.h"

// is the inventory open?
bool SlotSpace::open;
bool SlotSpace::update_called = false;

// which slots can be interacted with?
std::vector<SlotSet*> SlotSpace::slots_in_simulation;

sf::Color SlotSpace::ui_overlay_colour = sf::Color(23,24,28,120);

SlotSet* SlotSpace::backpack_slotset;
SlotSet* SlotSpace::hotbar_slotset;

HoveredSlot SlotSpace::hovered_data;