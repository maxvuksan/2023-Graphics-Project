#include "Inventory.h"
#include "../GameUI/SlotSpace.h"

int Inventory::row_length = 8;
int Inventory::row_count = 5;

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

    SlotSpace::Update(surface);

    int rows_to_render = 1;

    if (holding_item) {
        ItemDictionary::SetItemSprite(held_item_sprite, held_item);
        held_item_sprite.setPosition(
        sf::Vector2f(-4, -4) +
        sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y));
        surface.draw(held_item_sprite);
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

            SlotSpace::Clear();
            SlotSpace::SetOpen(!SlotSpace::Open());

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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            ShiftClickOnSlot();
        }
        else{
            LeftClickOnSlot();
        }
        break;
      }

      case sf::Mouse::Button::Right:
      
        RightClickOnSlot();
        break;
    }
  }
}

void Inventory::LeftClickOnSlot(){
    
    if(GetHoveredSlot()->type == SlotType::RECIPE){
        ClickOnRecipeSlot();
        return;
    }

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

    if(GetHoveredSlot()->type == SlotType::RECIPE){
        ClickOnRecipeSlot();
        return;
    }

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

void Inventory::ShiftClickOnSlot(){

    Slot* hovered = GetHoveredSlot();

    // ignore empty shift clicks
    if(!hovered->Occupied()){
        return;
    }

    SlotSet* current_slotset = SlotSpace::GetHovered().set_parent; 

    const std::vector<SlotSet*>& shift_container_1 = SlotSpace::GetShiftContainer1();
    std::vector<SlotSet*>* opposing_slotset = &SlotSpace::GetShiftContainer1();

    // establish which container we should move the item to
    for(int i = 0; i < shift_container_1.size(); i++){
        if(shift_container_1[i] == current_slotset){
            opposing_slotset = &SlotSpace::GetShiftContainer2();
            break;
        }
    }


    // look for an existing slot
    for(int i = 0; i < opposing_slotset->size(); i++){
        
        sf::Vector2i existing_slot = FindSlotContainingItem(opposing_slotset->at(i), hovered->item_code);

        if(existing_slot != sf::Vector2i(-1,-1)){
            opposing_slotset->at(i)->GetSlot(existing_slot.x, existing_slot.y)->count += hovered->count;
            hovered->count = 0;
        }
    }

    // find free slot
    
    for(int i = 0; i < opposing_slotset->size(); i++){
        
        sf::Vector2i free_slot = FindNextFreeSlot(opposing_slotset->at(i));

        if(free_slot != sf::Vector2i(-1,-1)){
            opposing_slotset->at(i)->GetSlot(free_slot.x, free_slot.y)->item_code = hovered->item_code;
            opposing_slotset->at(i)->GetSlot(free_slot.x, free_slot.y)->count += hovered->count;
            hovered->count = 0;
        }
    }
}

void Inventory::ClickOnRecipeSlot(){
    if(holding_item){
        return; // cannot purchase while holding item
    }

    const RecipeData& recipe = SlotSpace::GetHovered().set_parent->GetRecipe(SlotSpace::GetHovered().x, SlotSpace::GetHovered().y);

    if(HasIngredientsForRecipe(recipe)){

        holding_item = true;
        held_item = recipe.result.item_code;
        held_item_count = recipe.result.count;

        RemoveIngredientsForRecipe(recipe);
    }
}

bool Inventory::HasIngredientsForRecipe(const RecipeData& recipe){

    // iterates over each ingredient checking if the inventory slot sets contain enough
    for(int i = 0; i < recipe.ingredients.size(); i++){

        int quantity = CountItemInSlotSet(hotbar_slot_set, recipe.ingredients[i].item_code) + CountItemInSlotSet(backpack_slot_set, recipe.ingredients[i].item_code);

        if(recipe.ingredients[i].count > quantity){
            return false;
        }
    }
    return true;
}

int Inventory::CountItemInSlotSet(SlotSet* slot_set, ItemCode item){

    int count = 0;

    for (int y = 0; y < slot_set->GetRowCount(); y++) {
        for (int x = 0; x < slot_set->GetRowLength(); x++) {

            if (slot_set->GetSlot(x, y)->item_code == item) {
                count += slot_set->GetSlot(x,y)->count;
            }
        }
    }

    return count;
}

void Inventory::RemoveIngredientsForRecipe(const RecipeData& recipe){
    
    for(int i = 0; i < recipe.ingredients.size(); i++){

        int count = recipe.ingredients[i].count;

        RemoveItemFromSlotSet(backpack_slot_set, recipe.ingredients[i].item_code, &count);
        RemoveItemFromSlotSet(hotbar_slot_set, recipe.ingredients[i].item_code, &count);
    }
}

void Inventory::RemoveItemFromSlotSet(SlotSet* slot_set, ItemCode item_code, int* count_remaining){

    // dont search if we have no count remaining
    if(*count_remaining <= 0){
        return;
    }

    // iterate over each slot
    for (int y = 0; y < slot_set->GetRowCount(); y++) {
        for (int x = 0; x < slot_set->GetRowLength(); x++) {

            // slot matches the item_code
            if(slot_set->GetSlot(x, y)->item_code == item_code && slot_set->GetSlot(x,y)->Occupied()){

                if(slot_set->GetSlot(x, y)->count >= *count_remaining){
                    slot_set->GetSlot(x, y)->count -= (unsigned short)*count_remaining;
                    *count_remaining = 0;
                }
                else{ 
                    *count_remaining -= slot_set->GetSlot(x, y)->count;
                    slot_set->GetSlot(x, y)->count = 0;
                }

                if(*count_remaining == 0){
                    return;
                }
            }

        }
    }

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

void Inventory::PickupItem(ItemCode item, int count) {

    // should we keep looking?
    bool continue_searching = true;

    while(continue_searching){

        continue_searching = false;

        sf::Vector2i exisiting_stack = FindSlotContainingItem(hotbar_slot_set, item, true);

        // stack found
        if (exisiting_stack != sf::Vector2i(-1, -1)) {

            if(!AddItemToSlot(hotbar_slot_set, item, exisiting_stack.x, exisiting_stack.y, &count)){
                continue_searching = true;
            }
        }

        // keep looking, move to backpack
        exisiting_stack = FindSlotContainingItem(backpack_slot_set, item, true);

        if (exisiting_stack != sf::Vector2i(-1, -1)) {

            if(!AddItemToSlot(backpack_slot_set, item, exisiting_stack.x, exisiting_stack.y, &count)){
                continue_searching = true;
            }
        }

        if(count <= 0){
            return;
        }     
    }

    continue_searching = true;
    while(continue_searching){

        sf::Vector2i free_slot = FindNextFreeSlot(hotbar_slot_set);
        if (free_slot != sf::Vector2i(-1, -1)) {
            
            if(!AddItemToSlot(hotbar_slot_set, item, free_slot.x, free_slot.y, &count)){
                continue_searching = true;
            }
        }

        free_slot = FindNextFreeSlot(backpack_slot_set);
        
        if (free_slot != sf::Vector2i(-1, -1)) {
            
            if(!AddItemToSlot(backpack_slot_set, item, free_slot.x, free_slot.y, &count)){
                continue_searching = true;
            }
        }
        if(count <= 0){
            return;
        } 
    }

    // full
    std::cout << "ignoring item, inventory full... Inventory::PickupItem()";
}

int Inventory::GetItemInSelectedSlot() {

    Slot* slot = hotbar_slot_set->GetSlot(selected_slot, 0);

    if(slot->Occupied()){
        return slot->item_code;
    }
    return -1;
}

void Inventory::DecrementSelectedSlot() {
    hotbar_slot_set->GetSlot(selected_slot, 0)->count--;
}

sf::Vector2i Inventory::FindNextFreeSlot(SlotSet* slot_set) {

    for (int y = 0; y < slot_set->GetRowCount(); y++) {
        for (int x = 0; x < slot_set->GetRowLength(); x++) {
            if (!slot_set->GetSlot(x, y)->Occupied()) {
                return sf::Vector2i(x, y);
            }
        }
    }
    return sf::Vector2i(-1, -1);
}

sf::Vector2i Inventory::FindSlotContainingItem(SlotSet* slot_set, ItemCode item, bool ignore_full) {
  
    Slot* slot;

    for (int y = 0; y < slot_set->GetRowCount(); y++) {
        for (int x = 0; x < slot_set->GetRowLength(); x++) {

            slot = slot_set->GetSlot(x, y);

            if (slot->Occupied() && slot->item_code == item) {
                if(!ignore_full || slot->count < ItemDictionary::TYPE_STACK_SIZES[ItemDictionary::ITEM_DATA[item].type]){
                    return sf::Vector2i(x, y);
                }

            }
        }
    }
    return sf::Vector2i(-1, -1);
}

bool Inventory::AddItemToSlot(SlotSet* slot_set, ItemCode item, int x, int y, int* count_remaining) {

    Slot* slot = slot_set->GetSlot(x, y);
    int count_before = *count_remaining;

    short max_stack_size = ItemDictionary::TYPE_STACK_SIZES[ItemDictionary::ITEM_DATA[item].type];

    if (slot->item_code == item && slot->Occupied()) {
        slot->count += *count_remaining;
    } 
    else {
        slot->item_code = item;
        slot->count = *count_remaining;
    }

    // prevent item stacking overflow
    if(slot->count > max_stack_size){

        slot->count = max_stack_size;
        *count_remaining -= slot->count - max_stack_size;
    }
    else{
        *count_remaining = 0;
    }

    // clamp remaining count to 0
    if(*count_remaining < 0){
        *count_remaining = 0;
    }

    if(count_before == *count_remaining){
        return false;
    }
    return true;
}
