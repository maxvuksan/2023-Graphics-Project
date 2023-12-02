#include "GameClient.h"
#include "../World/World.h"
#include "../Player/PlayerController.h"
#include "../Player/Player.h"
#include "../ConsoleVisual.h"
#include "../Player/PlayerWorldInteractions.h"
#include "../Pathfinding/Fly.h"
#include "../Pathfinding/NoodleCreature.h"

sf::Vector2f GameClient::player_pos;

/*
    blocks multiplayer based code, when playing in single player
*/
#define IF_ONLINE if(!playing_online){return;}

void GameClient::LinkScene(Scene* scene){
    this->scene = scene;
}
void GameClient::LinkWorld(World* world){
    this->world = world;
}

void GameClient::CreateObjects(){
    

    // create world
    world->LinkClient(this);
    // pathfinding 
    //pathfinding_graph = scene->AddObject<PathfindingGraph>();
    //pathfinding_graph->LinkWorld(world);

    // setting scene bounds
    WorldProfile* wp = world->GetWorldProfile();
    scene->SetMinXBound(0);
    scene->SetMinYBound(0);
    scene->SetMaxXBound(wp->width * wp->tilemap_profile.width * wp->tilemap_profile.tile_width);
    scene->SetMaxYBound(wp->height * wp->tilemap_profile.height * wp->tilemap_profile.tile_height);

    // creating player and adding all player based components
    player = scene->AddObject<Player>();
    player_controller = player->AddComponent<PlayerController>();
    player_world_interactions = player->AddComponent<PlayerWorldInteractions>();
    player_world_interactions->LinkWorld(world);
    inventory = scene->AddUI<Inventory>();
    player_world_interactions->LinkInventory(inventory);
      


    scene->GetActiveCamera()->SetBackgroundTexture("background");

    // tell the world we want to focus around the player
    world->SetFocus(player->GetTransform());

    // create command console
    CommandParser::LinkClient(this);
    console_visual = scene->AddUI<ConsoleVisual>();

/*
    for(int i = 0; i < 40; i++){
       // Fly* fly = scene->AddObject<Fly>();
     //   fly->GetTransform()->position = sf::Vector2f(50,50);
    }
    //scene->AddObject<NoodleCreature>();
*/
}

void GameClient::SendPlayerControl(){

    player_pos = player->GetTransform()->position;

    IF_ONLINE

    if(player->GetTransform()->position != previous_player_position){
        
        previous_player_position = player->GetTransform()->position;
        SendPacket<p_PlayerControl>(server, {{PACKET_PlayerControl, client_id}, player->GetComponent<SpriteRenderer>()->GetFlip(), previous_player_position.x, previous_player_position.y });
    }
}

void GameClient::SendSetBlock(short tile_index, int x, int y){
    
    IF_ONLINE

    SendPacket<p_SetBlock>(server, {{PACKET_SetBlock, client_id}, tile_index, x, y});
}

void GameClient::SendChatMessage(const std::string& message){
    
    IF_ONLINE

    // having problems with strings in packets, ignore for now
    return;

    //char msg[50];
    //SendPacket<p_ChatMessage>(server, {{PACKET_ChatMessage, client_id}});
}


void GameClient::Update(){

    IF_ONLINE

    // if the server hasn't responded in a while, timeout / drop connection
    if(time_since_last_packet > timeout_limit){
        Disconnect();
    }

    time_since_last_packet++;
}

void GameClient::CatchPeerEvent(ENetEvent event){

    IF_ONLINE

    time_since_last_packet = 0;

    switch(event.type) 
    {   

        case ENET_EVENT_TYPE_RECEIVE: {

            InterpretPacket(event);

            enet_packet_destroy(event.packet);

            break;        
        }

    }
}



void GameClient::InterpretPacket(ENetEvent& event){
    
    IF_ONLINE

    // copy to the header portion of the message into PacketHeader object
    PacketHeader header;
    memcpy(&header, event.packet->data, sizeof(PacketHeader));

    // ignore messages from self
    if(header.client_id == client_id){
        return;
    }

    PACKET packet_type = (PACKET)header.type;

    switch(packet_type){

        case PACKET_SetClientId: {
            
            client_id = header.client_id;
            break;
        }
        case PACKET_CreatePlayer: {
            connected_clients[header.client_id] = scene->AddObject<Player>();
            break;
        }

        case PACKET_DeletePlayer: {

            scene->DeleteObject(connected_clients[header.client_id]);
            connected_clients.erase(header.client_id);
            break;
        }

        case PACKET_PlayerControl: {

            p_PlayerControl body;
            memcpy(&body, event.packet->data, sizeof(p_PlayerControl));

            connected_clients[header.client_id]->GetComponent<SpriteRenderer>()->SetFlip(body.flip_sprite);
            connected_clients[header.client_id]->GetTransform()->position = sf::Vector2f(body.pos_x, body.pos_y);

            break;
        }

        case PACKET_SetBlock : {
            p_SetBlock body;
            memcpy(&body, event.packet->data, sizeof(p_SetBlock));

            world->SetTile(body.tile_index, body.pos_x, body.pos_y, SetLocation::MAIN, SetMode::OVERRIDE, false);
            
            break;
        }

        case PACKET_ChatMessage : {
            


            break;
        }
    }
}

void GameClient::OnDisconnect(){

    IF_ONLINE
     
    //scene->DeleteObject(world);
}