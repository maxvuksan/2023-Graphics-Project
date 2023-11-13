#include "CommandParser.h"
#include "../Amber/Framework.h"
#include "Networking/GameClient.h"
#include "PlayerController.h"
#include <cstring>
#include "ConsoleVisual.h"

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

    // removing '/' 
    cmd_raw = cmd_raw.substr(1, cmd_raw.length()); 

    if(cmd_raw == "RESPAWN"){
        client->player_controller->Respawn();
        return {""};
    }
    else if(cmd_raw == "DEBUG"){
        Core::DEBUG_MODE = !Core::DEBUG_MODE;
        return {""};
    }
    else if(cmd_raw == "CLEAR"){
        client->console_visual->ClearLines();
        return {""};
    }
    else if(cmd_raw == "FLY"){
        client->player_controller->SetFlyMode(!client->player_controller->GetFlyMode());
        return {""};
    }
    else if(cmd_raw == "POSITION"){
        return {"Player Position : {" + std::to_string(client->player->GetTransform()->GetGlobalPosition().x) + ", " + std::to_string(client->player->GetTransform()->GetGlobalPosition().y) + "}" ,debug_grey};
    }
    else if(cmd_raw == "HELP"){
        return {"/HELP, /RESPAWN, /CLEAR, /DEBUG, /FLY, /POSITION", Globals::DEBUG_COLOUR};
    }

    return {"'/" + cmd_raw + "' is not a command, see /HELP", sf::Color(235, 124, 124)};

}
