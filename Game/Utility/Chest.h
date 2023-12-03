#include "UtilityStation.h"
#include "../GameUI/SlotSet.h"
class Chest : public UtilityStation{

    public:

        void OnStart();
        void OnOpen() override;
        void OnClose() override;

    private:

        SlotSet* slot_set;

};

