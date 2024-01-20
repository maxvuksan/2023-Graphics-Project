#include "../../Amber/Framework.h"

/*
    responsible for drawing shadows masked to the background tilemap,
*/
class BackgroundShadowManager : public Object {

    public:

        void Start() override;
        void Draw(sf::RenderTarget& surface) override;


};