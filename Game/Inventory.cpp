#include "Inventory.h"

void Inventory::Start(){

    slots.resize(row_count);
    for(int i = 0; i < slots.size(); i++){
        slots[i].resize(row_length);
    }

}