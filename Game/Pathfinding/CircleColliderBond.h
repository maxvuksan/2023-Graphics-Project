#include "../../Amber/Framework.h"

class CircleColliderBond : public Object {


    // determines how mass points interact with each other
    struct BindingProfile{
        Object* mass_object;
        int radius;
        float desired_distance;
        CircleCollider* circle_collider;
        PhysicsBody* physics_body;
    };

    public:

        void Start() override;
        // moves all mass points to a new position
        void SetPosition(sf::Vector2f position);
        void Update();

        void Draw(sf::RenderTarget& surface);
        sf::Vector2f PositionOnMassPointEdge(int mass_point, float radians);

    private:

        sf::VertexArray skin;

        float gap_between_points = 2;
        std::vector<BindingProfile> mass_points;
};