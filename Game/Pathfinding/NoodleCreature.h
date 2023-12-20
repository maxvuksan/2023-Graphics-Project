#include "../../Amber/Framework.h"

class World;
class InverseKinematicLimb;
class NoodleCreature : public Object {

    public:
        
        void LinkWorld(World* world);

        void Start() override;

        void Draw(sf::RenderTarget& surface) override;

        sf::Vector2i RandomCoordinateNearTarget(sf::Vector2f& target_position);
        bool GrabCoordinateIsValid(sf::Vector2i& grab_coordinate);

    private: 
        World* world;
        PhysicsBody* physics;

        std::vector<InverseKinematicLimb*> limbs;
        std::vector<int> ideal_grab_angle;

        std::vector<sf::Vector2i> path_to_target; 

        float search_again_delay = 3;
        float search_again_delay_tracked = 0;
        int current_node = 0;
};