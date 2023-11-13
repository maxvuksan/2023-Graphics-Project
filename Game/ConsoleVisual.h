#pragma once
#include "../Amber/Framework.h"
#include "CommandParser.h"

class GameClient;
/*
    provides a visual interface to interact with the CommandParser
*/
class ConsoleVisual : public Object {

    public:
        ConsoleVisual();
        ~ConsoleVisual();

        void LinkClient(GameClient* client);

        void Start() override;
        void Draw(sf::RenderTarget& texture) override;

        void CatchEvent(sf::Event event) override;
        void Print(const ConsoleLine& message, bool send_packet = true);

        void ClearLines();

    private:
        int line_count = 15;
        std::vector<sf::Text> lines;
        
        // enforcing singleton behavior 
        static ConsoleVisual* instance;
        
        sf::Color interact_background_colour;
        sf::Text interact_line;
        sf::RectangleShape interact_box;

        bool active;

        GameClient* client;
};