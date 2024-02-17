#include "../../Amber/Framework.h"

class Heavy : public Object {

    public:

        void Start() override{

            AddComponent<SpriteRenderer>()->SetTexture("heavy");
            AddComponent<PhysicsBody>();
            AddComponent<BoxCollider>();
        }



};
