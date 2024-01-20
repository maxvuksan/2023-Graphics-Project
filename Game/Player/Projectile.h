#include "../../Amber/Framework.h"

class Projectile : public Object {

    public:
        void Start() override;
        void Update() override;

    private:
        RotatingRect* rotating_rect;
        AnimationRenderer* animation_renderer;
        float speed;
        float inital_speed;
        float decay_speed;

};