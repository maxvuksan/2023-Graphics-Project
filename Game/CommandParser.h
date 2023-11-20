#pragma once
#include <iostream>
#include "Player/Player.h"
#include "World/World.h"

class GameClient;

/*
    allows basic commands to aid debugging and development, generally interacted with the ConsoleVisual class
*/

struct ConsoleLine{
    std::string message;
    sf::Color colour = sf::Color::White;
};

class CommandParser {

    public: 

        static void LinkClient(GameClient* client);

        /*
            executes code interpreted by the given command, 

            commands begin with '/'

            if no '/' is present the command will be sent as a chat message
        */
        static ConsoleLine Execute(std::string command);

    private:
        
        static GameClient* client;

};