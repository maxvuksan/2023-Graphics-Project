#include "CraftingStation.h"
#include "../GameUI/SlotSpace.h"


void CraftingStation::SetRecipeGroup(RecipeGroups recipe_group){
    
    slot_set->DefineGrid(ItemDictionary::RECIPE_GROUP[recipe_group].width, ItemDictionary::RECIPE_GROUP[recipe_group].height, SlotType::RECIPE);
    slot_set->DefineRecipeGrid(ItemDictionary::RECIPE_GROUP[recipe_group].recipes);

}


void CraftingStation::OnOpen(){
    slot_set->SetActive(true);
    SlotSpace::Include(slot_set);
    SlotSpace::SetOpen(true);
}   

void CraftingStation::OnClose(){
    slot_set->SetActive(false);
    SlotSpace::SetOpen(false);
}