#include "Container.h"
#include "../GameUI/SlotSpace.h"



void Container::OnOpen(){
    slot_set->SetActive(true);
    SlotSpace::Include(slot_set);
    SlotSpace::SetOpen(true);
}   

void Container::OnClose(){
    slot_set->SetActive(false);
    SlotSpace::SetOpen(false);
}