#include "Container.h"


class CraftingStation : public UtilityStation {
    
    public:
        void OnStart() override;
        void OnOpen() override;
        void OnClose() override;

        void SetRecipeGroup(RecipeGroups recipe_group);

    private:
        SlotSet* slot_set;
        
};