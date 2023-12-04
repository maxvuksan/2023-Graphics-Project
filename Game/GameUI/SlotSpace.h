#include "../../Amber/Framework.h"
#include "SlotSet.h"

/*
    the interactive state of the inventory, (e.g what slots am i interacting with?)
*/
class SlotSpace {


    public:

        // which slots can be interacted with?
        static std::vector<SlotSet*> slots_in_simulation;

        static sf::Color ui_overlay_colour;

        static void DefineInventorySlotsets(SlotSet* hotbar, SlotSet* backpack){
            hotbar_slotset = hotbar;
            backpack_slotset = backpack;
        }

        static void Clear(){

            for(auto slot_set : slots_in_simulation){
                slot_set->SetActive(false);
            }
        }

        static void Include(SlotSet* slot){
            slots_in_simulation.push_back(slot);
        }

        static void SetOpen(bool _open){
            open = _open;

            if(open){
                Scene::GetActiveCamera()->ui_overlay_colour = ui_overlay_colour;
                backpack_slotset->SetActive(true);

                if(slots_in_simulation.size() == 0){
                    backpack_slotset->Align(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
                }
                else{

                    std::cout << backpack_slotset->GetWidth() << " width\n";

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
        static void Update(){
            update_called = true;

            if(backpack_slotset == nullptr || hotbar_slotset == nullptr ){
                std::cout << "ERROR : Inventory slot sets have not been assigned, SlotSpace::Update\n";
                return;
            }

            std::vector<SlotSet*> possible_slots = slots_in_simulation;
            possible_slots.push_back(hotbar_slotset);
            possible_slots.push_back(backpack_slotset);
            hovered_data = SlotSet::GetHoveredSlotFromMultipleSets(possible_slots);
        }


    private:
        // is the inventory open?
        static bool open;
        static bool update_called;
        static HoveredSlot hovered_data;
        
        static SlotSet* backpack_slotset;
        static SlotSet* hotbar_slotset;
};