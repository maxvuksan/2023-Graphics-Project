#include "Chest.h"

void Chest::OnStart(){

    slot_set = chunk->AddUIToChunk<SlotSet>();
    slot_set->DefineGrid(8,2, SlotType::OPEN);
    slot_set->SetActive(false);
    slot_set->SetAlign(ScreenLocation::CENTER);
}

void Chest::OnOpen(){
    slot_set->SetActive(true);
}   

void Chest::OnClose(){
    slot_set->SetActive(false);
}