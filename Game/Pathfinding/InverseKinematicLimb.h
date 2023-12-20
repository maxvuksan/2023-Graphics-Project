#include "../../Amber/Framework.h"

/*
    representing a procedual limb, operating through inverse kinematics,

    the start position of the limb will follow the objects transform, 
    while the end can be locked / bounded to a set location

*/
class InverseKinematicLimb : public Component{

    struct Segment {
        int length = 0;
        float angle = 0; // in radians
        Segment* parent = nullptr;

        sf::Vector2f start;
        sf::Vector2f end;

        void CalculateStartPosition();
        void Follow(sf::Vector2f target);
        void Translate(sf::Vector2f translation);
        void Update();
    };

    public:

        void Init(int thickness, int count, int length_per_segment);

        void SetTargetOverTime(sf::Vector2f target, float time);
        void SetTarget(sf::Vector2f target);
        void SetOrigin(sf::Vector2f origin);

        sf::Vector2f GetTarget() {return target;}
        sf::Vector2f GetOrigin() {return origin;}

        // @returns true if the limb has a fixed target, false if within transition (From SetTargetOverTime)
        bool GetIsAttached(){return attached_to_a_target;}

        void AddSubLimb(int start, int end, int thickness, int count, int length_per_segment);

        void Start() override;
        void Draw(sf::RenderTarget& surface) override;
    

    private:
        int thickness;
        int count;
        int length;

        sf::Vector2f original_target;
        sf::Vector2f next_target;
        float time_tracked;
        float time_total;
        bool attached_to_a_target;

        std::vector<Segment> segments;

        std::vector<InverseKinematicLimb*> sublimbs;
        std::vector<std::pair<int,int>> sublimb_contact_points; // structured as { start index, end index }

        sf::Vector2f target;
        sf::Vector2f origin;
};