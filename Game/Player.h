#include "../Amber/Framework.h"

class Player : public Object{

    public:

        void Start() override;
        void Update() override;
        void CatchEvent(sf::Event) override;

    private:

        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;

        PhysicsBody* pb;
        float jump_height = 0.15;

};