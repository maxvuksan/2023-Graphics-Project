#include "../../Amber/Framework.h"

class Projectile : public Object {

    public:
        void Start() override;
        void Update() override;

    private:
        SpriteRenderer* sr;
        float speed;
        float inital_speed;
        float decay_speed;

};