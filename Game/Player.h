#include "../Amber/Framework.h"

class World;
class Player : public Object{

    public:

        void Start() override;
        void Update() override;
        void CatchEvent(sf::Event) override;

        void LinkWorld(World* world){this->world = world;}

    private:

        World* world;
        sf::Vector2i focused_block;

        BoxCollider* ground;
        BoxCollider* left;
        BoxCollider* right;

        PhysicsBody* pb;
        float jump_height = 0.15;

};