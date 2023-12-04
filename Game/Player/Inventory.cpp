#include "Inventory.h"
#include "../GameUI/SlotSpace.h"

int Inventory::row_length = 8;
int Inventory::row_count = 6;

void Inventory::Start() {


    selected_slot = 0;

    SetRenderLayer(1);

    hotbar_slot_set = GetScene()->AddUI<SlotSet>();
    backpack_slot_set = GetScene()->AddUI<SlotSet>();

    hotbar_slot_set->DefineGrid(row_length, 1, SlotType::OPEN);
    backpack_slot_set->DefineGrid(row_length, row_count - 1, SlotType::CLOSED);

    hotbar_slot_set->Align(ScreenLocationX::CENTER, ScreenLocationY::BOTTOM);

    SlotSpace::DefineInventorySlotsets(hotbar_slot_set, backpack_slot_set);
    SlotSpace::SetOpen(false);

    holding_item = false;

   // PickupItem(ItemCode::item_Utility_WorkBench);
    PickupItem(ItemCode::item_Gold_Picaxe);
    PickupItem(ItemCode::item_Copper_Picaxe);
    PickupItem(ItemCode::item_Utility_Furnace);
    PickupItem(ItemCode::item_Utility_Chest);
    PickupItem(ItemCode::item_BigLeaf);
    PickupItem(ItemCode::item_Fibre);
    //PickupItem(ItemCode::item_Fibre);

    PickupItem(ItemCode::item_Main_StonePlate);
    PickupItem(ItemCode::item_Gold);
    PickupItem(ItemCode::item_Gold);
    PickupItem(ItemCode::item_Gold);

    SetSelectedSlotSprite();

}

void Inventory::Draw(sf::RenderTarget& surface) {

    SlotSpace::Update();

    int rows_to_render = 1;

    if (holding_item) {
        ItemDictionary::SetItemSprite(held_item_sprite, held_item);
        held_item_sprite.setPosition(
        sf::Vector2f(-4, -4) +
        sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y));
        surface.draw(held_item_sprite);
    }
}

Slot* Inventory::GetInventorySlot(int x, int y){
    if(y == 0){
        return hotbar_slot_set->GetSlot(x,0);
    }
    else{
        return backpack_slot_set->GetSlot(x,y - 1);
    }
}

Slot* Inventory::GetHoveredSlot(){
    return SlotSpace::GetHovered().set_parent->GetSlot(SlotSpace::GetHovered().x, SlotSpace::GetHovered().y);
}


void Inventory::CatchEvent(sf::Event event) {
  if (event.type == sf::Event::KeyPressed) {

    int previous_selected = selected_slot;

    switch (event.key.scancode) {

        // open inventory
        case sf::Keyboard::Scan::E:
        
        SlotSpace::SetOpen(true);

        break;

        // close inventory
        case sf::Keyboard::Scan::Escape:
            SlotSpace::Clear();
            SlotSpace::SetOpen(false);
            break;
    
        // choose selected slot
        case sf::Keyboard::Scan::Num1:
        case sf::Keyboard::Scan::Num2:
        case sf::Keyboard::Scan::Num3:
        case sf::Keyboard::Scan::Num4:
        case sf::Keyboard::Scan::Num5:
        case sf::Keyboard::Scan::Num6:
        case sf::Keyboard::Scan::Num7:
        case sf::Keyboard::Scan::Num8:
            selected_slot = event.key.scancode - 26;
            break;


            
    }

    // allows scrolling through hotbar
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0) // moving up
        {
            selected_slot++;
        }
        else{
            selected_slot--;
        }
        selected_slot = selected_slot % row_length;
    }

    if(previous_selected != selected_slot){
        SetSelectedSlotSprite();
    }

  }
  // only procceed if inventory open
  if(!SlotSpace::Open()){
    return;
  }

  if (event.type == sf::Event::MouseButtonPressed) {
    
    if (SlotSpace::GetHovered().set_parent == nullptr) {
        // we are not hovering something, in future maybe drop item?
        return;
    }


    switch (event.mouseButton.button) {
      
      case sf::Mouse::Button::Left: {

        LeftClickOnSlot();
        break;
      }

      case sf::Mouse::Button::Right:
      
        RightClickOnSlot();
        break;
    }
  }
}

void Inventory::LeftClickOnSlot(){
    // swap with slot
    if (GetHoveredSlot()->Occupied()) {

        ItemCode was_in_hand = held_item;
        unsigned int was_in_hand_count = held_item_count;

        held_item = GetHoveredSlot()->item_code;
        held_item_count = GetHoveredSlot()->count;
        GetHoveredSlot()->count = 0; // taking item out

        bool _hlding_item = holding_item;
        holding_item = true;

        // swap with held item
        if (_hlding_item) {

            // items are the same, join stack
            if(was_in_hand == held_item){
                GetHoveredSlot()->count += was_in_hand_count;
                holding_item = false;
            }
            else{ // swap, they are different items
                GetHoveredSlot()->item_code = was_in_hand;
                GetHoveredSlot()->count = was_in_hand_count;    
                            
            }
        }

    } 
    else if (holding_item) {
        GetHoveredSlot()->item_code = held_item;
        GetHoveredSlot()->count = held_item_count;

        holding_item = false;
    }
    else{
        return;
    }    
    Sound::Play("noisy_blip");


}

void Inventory::RightClickOnSlot(){
    // swap with slot
    if (!holding_item && GetHoveredSlot()->Occupied()) {

        // take half of stack
        int count_to_take = ceil(GetHoveredSlot()->count / 2.0f);
        held_item = GetHoveredSlot()->item_code;
        held_item_count = count_to_take;
        holding_item = true;

        // remove what we are taking from the stack
        GetHoveredSlot()->count -= count_to_take;

        Sound::Play("noisy_blip");
        return;
    }
    // otherwise treat as left click
    LeftClickOnSlot();

     
}

void Inventory::SetSelectedSlotSprite(){
    for(int i = 0; i < row_length; i++){

        if(i == selected_slot){
            hotbar_slot_set->GetSlot(i, 0)->sprite = SlotSprite::SELECTED;
        }
        else{
            hotbar_slot_set->GetSlot(i, 0)->sprite = SlotSprite::DEFAULT;
        }
    }
}

void Inventory::PickupItem(ItemCode item) {
  sf::Vector2i exisiting_stack = FindSlotContainingItem(item);
  // stack found
  if (exisiting_stack != sf::Vector2i(-1, -1)) {
    AddItemToSlot(item, exisiting_stack.x, exisiting_stack.y);

    return;
  }
  // find free slot
  else {
    sf::Vector2i free_slot = FindNextFreeSlot();
    if (free_slot != sf::Vector2i(-1, -1)) {
      AddItemToSlot(item, free_slot.x, free_slot.y);
    }

    return;
  }
  // full
  std::cout << "ignoring item, inventory full... Inventory::PickupItem()";
}

int Inventory::GetItemInSelectedSlot() {

    Slot* slot = GetInventorySlot(selected_slot, 0);

    if(slot->Occupied()){
    return slot->item_code;
    }
    return -1;
}

void Inventory::DecrementSelectedSlot() {
    GetInventorySlot(selected_slot, 0)->count--;
}

sf::Vector2i Inventory::FindNextFreeSlot() {
  for (int y = 0; y < row_count; y++) {
    for (int x = 0; x < row_length; x++) {
      if (!GetInventorySlot(x, y)->Occupied()) {
        return sf::Vector2i(x, y);
      }
    }
  }
  return sf::Vector2i(-1, -1);
}

sf::Vector2i Inventory::FindSlotContainingItem(ItemCode item) {
  
  Slot* slot;

  for (int y = 0; y < row_count; y++) {
    for (int x = 0; x < row_length; x++) {

        slot = GetInventorySlot(x, y);

      if (slot->Occupied() && slot->item_code == item) {
        return sf::Vector2i(x, y);
      }
    }
  }
  return sf::Vector2i(-1, -1);
}

void Inventory::AddItemToSlot(ItemCode item, int x, int y) {

  Slot* slot = GetInventorySlot(x, y);

  if (slot->item_code == item && slot->Occupied()) {
    slot->count++;
  } else {
    slot->item_code = item;
    slot->count = 1;
  }
}
