#include "SlotSpace.h"

// is the inventory open?
bool SlotSpace::open;
bool SlotSpace::update_called = false;

std::vector<SlotSet*> SlotSpace::slots_in_simulation;

sf::Color SlotSpace::ui_overlay_colour = sf::Color(13,14,18,220);

SlotSet* SlotSpace::backpack_slotset;
SlotSet* SlotSpace::hotbar_slotset;

HoveredSlot SlotSpace::hovered_data;

std::vector<SlotSet*> SlotSpace::shift_container_1;
std::vector<SlotSet*> SlotSpace::shift_container_2;

sf::Text SlotSpace::hovered_item_name;
sf::Text SlotSpace::hovered_recipe_ingredient_text;