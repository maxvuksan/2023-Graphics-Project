#include "../../Amber/Framework.h"

class World;
class RotatedRectManager : public Object {

    public:

        static void LinkWorld(World* world);

        static void AddRect(RotatingRect* rect);
        static void RemoveRect(RotatingRect* rect);

        void Update() override;

    private:

        static World* world;
        static std::vector<RotatingRect*> rotated_rects;

};