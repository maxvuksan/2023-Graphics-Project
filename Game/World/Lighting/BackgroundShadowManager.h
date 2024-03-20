#pragma once
#include "../../Amber/Framework.h"

/*
    responsible for drawing shadows masked to the background tilemap,
*/
class BackgroundShadowManager : public Object {

    public:

        void Draw(sf::RenderTarget& surface) override;


};