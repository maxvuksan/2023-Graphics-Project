#include "Chest.h"
#include "../GameUI/SlotSpace.h"

void Chest::OnStart(){

    slot_set = chunk->AddUIToChunk<SlotSet>();
    slot_set->DefineGrid(8,2, SlotType::OPEN);
    slot_set->SetActive(false);
}

void Chest::OnOpen(){
    slot_set->SetActive(true);
    SlotSpace::Include(slot_set);
    SlotSpace::SetOpen(true);
}   

void Chest::OnClose(){
    slot_set->SetActive(false);
    SlotSpace::SetOpen(false);
}