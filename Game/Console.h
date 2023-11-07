#pragma once
#include "../Amber/Framework.h"

class Console : public Object {

    public:
        void Start() override;
        void Draw(sf::RenderTarget& texture) override;

        void CatchEvent(sf::Event event) override;

    private:
        int line_count = 15;
        std::vector<sf::Text> lines;
        sf::Text interact_line;
        
        sf::RectangleShape interact_box;

        bool active;
};