#include "Container.h"
#include "../GameUI/SlotSpace.h"

void Container::OnStart(){

    slot_set = chunk->AddUIToChunk<SlotSet>();
    slot_set->DefineGrid(8,2, SlotType::OPEN);
    slot_set->SetActive(false);
}


void Container::OnOpen(){
    slot_set->SetActive(true);
    SlotSpace::Include(slot_set);
    SlotSpace::SetOpen(true);
}   

void Container::OnClose(){
    slot_set->SetActive(false);
    SlotSpace::SetOpen(false);
}