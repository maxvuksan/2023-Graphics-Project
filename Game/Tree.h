#include "../Amber/Framework.h"

class Tree : public Object {

    public:

        void Start() override{
            auto sr = AddComponent<SpriteRenderer>();
            sr->SetTexture("tree");    
        }

        void Update() override{

        }
};