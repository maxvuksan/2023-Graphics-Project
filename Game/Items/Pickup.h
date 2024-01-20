#include "../../Amber/Framework.h"
#include "ItemDictionary.h"

class Pickup : public Object { 

    public:
        Pickup();
        void Start();

        // assigns the type of the pickup
        void SetItemCode(ItemCode item);
        // @returns the type of the pickup
        ItemCode GetItemCode();

        void AttractToTransform(Transform* target);
        void Update();

    private:
        Transform* target;
        ItemCode item_code;
        SpriteRenderer* sr;

        sf::Vector2f on_attract_position;


        static float popin_speed;
        float popin_speed_tracked;

        static float attract_speed; // lerp by
        float attract_tracked; // the lerp value
};