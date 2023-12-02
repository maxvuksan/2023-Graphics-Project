#include "../../Amber/Framework.h"

class NoodleCreature : public Object {

    public:
        
        void Start() override;

        void Update() override;

    private: 
        PhysicsBody* physics;

        std::vector<sf::Vector2i> path_to_target; 

        float search_again_delay = 3;
        float search_again_delay_tracked = 0;
        int current_node = 0;
};