#include "CommandParser.h"
#include "../Amber/Framework.h"
#include "Networking/GameClient.h"
#include "Player/PlayerController.h"
#include <cstring>
#include "ConsoleVisual.h"
#include <sstream>

GameClient* CommandParser::client = nullptr;

void CommandParser::LinkClient(GameClient* _client){
    client = _client;
}

ConsoleLine CommandParser::Execute(std::string cmd_raw){


    // is chat message
    if(cmd_raw.size() == 0 || cmd_raw[0] != '/'){
        return {cmd_raw};
    }

    sf::Color debug_grey = sf::Color(200,200,200);
    sf::Color debug_error = sf::Color(235, 124, 124);

    // removing '/' 
    cmd_raw = cmd_raw.substr(1, cmd_raw.length()); 

    // split into tokens
    std::stringstream ss(cmd_raw);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);


    if(tokens[0] == "RESPAWN"){
        client->player_controller->Respawn();
        return {""};
    }
    else if(tokens[0] == "DEBUG"){
        Core::DEBUG_MODE = !Core::DEBUG_MODE;
        return {""};
    }
    else if(tokens[0] == "CLEAR"){
        client->console_visual->ClearLines();
        return {""};
    }
    else if(tokens[0] == "FLY"){
        client->player_controller->SetFlyMode(!client->player_controller->GetFlyMode());
        return {""};
    }
    else if(tokens[0] == "FPS"){
        Core::SHOW_FPS = !Core::SHOW_FPS;
        return {""};
    }
    else if(tokens[0] == "SHOWMAP"){
        client->world->minimap->reveal_all = !client->world->minimap->reveal_all;
        return {""};
    }
    else if(tokens[0] == "HELP"){
        return {"/HELP, /RESPAWN, /CLEAR, /DEBUG, /FLY, /FPS, /SHOWMAP, /GIVE", Globals::DEBUG_COLOUR};
    }

    else if(tokens[0] == "GIVE"){
        if(tokens.size() <= 1){
            return {"'/GIVE' requires additonal arguments, '/GIVE item_code quantity=1", debug_error};
        }

        int item_code = std::stoi(tokens[1]);

        if(item_code < 0 || item_code > item_NUMBER_OF_ITEMS){
            return {"The item_code provided is invalid", debug_error};
        }

        int count = 1;

        if(tokens.size() > 2){

            count = std::stoi(tokens[2]);

            if(count < 0 || count > 999){
                return {"The quantity provided is invalid", debug_error};
            }
        }
        client->inventory->PickupItem((ItemCode)item_code, count);
        return {"Given ItemCode::" + std::to_string(item_code), debug_grey};
    }

    return {"'/" + cmd_raw + "' is not a command, see /HELP", debug_error};

}
