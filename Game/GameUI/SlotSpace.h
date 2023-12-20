#include "../../Amber/Framework.h"
#include "SlotSet.h"

/*
    the interactive state of the inventory, (e.g what slots am i interacting with?)
*/
class SlotSpace {


    public:

        static sf::Color ui_overlay_colour;

        // links references to the hotbar and backpack slotsets
        static void DefineInventorySlotsets(SlotSet* hotbar, SlotSet* backpack){
            hotbar_slotset = hotbar;
            backpack_slotset = backpack;

            hovered_item_name.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));
            hovered_item_name.setFont(*AssetManager::GetFont("Amber_Default"));
            hovered_item_name.setCharacterSize(8);

            hovered_recipe_ingredient_text.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding + 15));
            hovered_recipe_ingredient_text.setFont(*AssetManager::GetFont("Amber_Default"));
            hovered_recipe_ingredient_text.setCharacterSize(8);
        }

        static void Clear(){

            for(auto slot_set : slots_in_simulation){
                slot_set->SetActive(false);
            }
            slots_in_simulation.clear();
        }

        // allow a slot set to be interacted with
        static void Include(SlotSet* slot){
            slots_in_simulation.push_back(slot);
            slot->SetActive(true);
        }

        static void SetOpen(bool _open){
            open = _open;

            shift_container_1.clear();
            shift_container_2.clear();

            if(open){
                Scene::GetActiveCamera()->ui_overlay_colour = ui_overlay_colour;
                backpack_slotset->SetActive(true);

                shift_container_1.push_back(hotbar_slotset);

                if(slots_in_simulation.size() == 0){
                    backpack_slotset->Align(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
                    
                    shift_container_2.push_back(backpack_slotset);
                }
                else{
                    
                    shift_container_1.push_back(backpack_slotset);
                    shift_container_2.push_back(slots_in_simulation[0]);

                    // alignment... 
                    
                    UIRect::AlignX(backpack_slotset->GetTransform(), backpack_slotset->GetWidth(), ScreenLocationX::CENTER);
                    UIRect::AlignX(slots_in_simulation[0]->GetTransform(), slots_in_simulation[0]->GetWidth(), ScreenLocationX::CENTER);

                    int height = backpack_slotset->GetHeight() + UIRect::padding + slots_in_simulation[0]->GetHeight();
                    
                    UIRect::AlignY(slots_in_simulation[0]->GetTransform(), height, ScreenLocationY::CENTER);
                    backpack_slotset->GetTransform()->position.y = slots_in_simulation[0]->GetTransform()->position.y + slots_in_simulation[0]->GetHeight() + UIRect::padding;


                                      
                }
            }
            else{
                Scene::GetActiveCamera()->ui_overlay_colour = sf::Color::Transparent;
                backpack_slotset->SetActive(false);

                Clear();
            }
        }

        static bool Open(){
            return open;
        }

        // gets the hovered slot checking all included slots
        static const HoveredSlot& GetHovered(){
            return hovered_data;
        }

        // must be called somewhere
        static void Update(sf::RenderTarget& surface){
            update_called = true;

            if(backpack_slotset == nullptr || hotbar_slotset == nullptr ){
                std::cout << "ERROR : Inventory slot sets have not been assigned, SlotSpace::Update\n";
                return;
            }

            if(!open){
                return;
            }

            // determine hovered slot
            std::vector<SlotSet*> possible_slots = slots_in_simulation;
            possible_slots.push_back(hotbar_slotset);
            possible_slots.push_back(backpack_slotset);
            hovered_data = SlotSet::GetHoveredSlotFromMultipleSets(possible_slots);

            // assign the hovered tool tip
            if(hovered_data.set_parent != nullptr){
                hovered_item_name.setString(
                    ItemDictionary::ITEM_DATA[hovered_data.set_parent->GetSlot(hovered_data.x, hovered_data.y)->item_code].name);


                surface.draw(hovered_item_name);

                if(hovered_data.set_parent->GetSlot(hovered_data.x, hovered_data.y)->type == SlotType::RECIPE){
                    
                    const RecipeData& recipe = hovered_data.set_parent->GetRecipe(hovered_data.x, hovered_data.y);
                    
                    hovered_recipe_ingredient_text.setString("");
                    // iterate over each ingredient, adding it as a line in the text object
                    for(int i = 0; i < recipe.ingredients.size(); i++){
                                                                // count'x' name 
                        hovered_recipe_ingredient_text.setString(hovered_recipe_ingredient_text.getString() + std::to_string(recipe.ingredients[i].count) + "x" + ItemDictionary::ITEM_DATA[recipe.ingredients[i].item_code].name);
                    }
                    
                    surface.draw(hovered_recipe_ingredient_text);

                }
            }
        }

        // shift containers determine where items are moved to when they are shifted clicked, first it attempts to move it to the opposing container
        // e.g. container 1 -> container 2 otherwise looks for alternate slot sets in its own container that have a free slot
        static std::vector<SlotSet*>& GetShiftContainer1(){
            return shift_container_1;
        }
        static std::vector<SlotSet*>& GetShiftContainer2(){
            return shift_container_2;
        }

    private:

        // is the inventory open?
        static bool open;
        static bool update_called;
        static HoveredSlot hovered_data;

        // which slots can be interacted with?
        static std::vector<SlotSet*> slots_in_simulation;

        
        static SlotSet* backpack_slotset;
        static SlotSet* hotbar_slotset;

        // used to determine where shifted items are moved to
        static std::vector<SlotSet*> shift_container_1;
        static std::vector<SlotSet*> shift_container_2;

        static sf::Text hovered_item_name;
        static sf::Text hovered_recipe_ingredient_text;
};