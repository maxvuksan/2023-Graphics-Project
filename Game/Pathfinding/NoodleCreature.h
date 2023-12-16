#include "../../Amber/Framework.h"

class InverseKinematicLimb;
class NoodleCreature : public Object {

    public:
        
        void Start() override;

        void Update() override;

    private: 
        PhysicsBody* physics;

        std::vector<InverseKinematicLimb*> limbs;
        std::vector<int> ideal_grab_angle;

        std::vector<sf::Vector2i> path_to_target; 

        float search_again_delay = 3;
        float search_again_delay_tracked = 0;
        int current_node = 0;
};