#include "Container.h"


class CraftingStation : public UtilityStation {
    
    public:
        void OnOpen() override;
        void OnClose() override;

        void SetRecipeGroup(RecipeGroups recipe_group);

        
};