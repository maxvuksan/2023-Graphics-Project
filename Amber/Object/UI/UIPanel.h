#pragma once
#include "UIRect.h"

/*
    a solid colour overlay covering the entire screen,
*/
class UIPanel: public Component {

    public:

        void Start();

        void Draw(sf::RenderTarget& surface);
        void SetColour(const sf::Color& colour);

        void OnResize() override;
 
    private:

        sf::RectangleShape rectangle;

};