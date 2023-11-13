#include "GameClient.h"
#include "../World.h"
#include "../PlayerController.h"
#include "../Player.h"
#include "../ConsoleVisual.h"

void GameClient::LinkScene(Scene* scene){
    this->scene = scene;
}

void GameClient::CreateObjects(){
    
    CommandParser::LinkClient(this);

    world = scene->AddObject<World>();
    world->LinkClient(this);

    // setting scene bounds
    WorldProfile* wp = world->GetWorldProfile();
    scene->SetMinXBound(0);
    scene->SetMinYBound(0);
    scene->SetMaxXBound(wp->width * wp->tilemap_profile.width * wp->tilemap_profile.tile_width);
    scene->SetMaxYBound(wp->height * wp->tilemap_profile.height * wp->tilemap_profile.tile_height);

    // creating player
    player = scene->AddObject<Player>();
    player_controller = player->AddComponent<PlayerController>();
    player_controller->LinkWorld(world);
    
    world->SetFocus(player->GetTransform());

    console_visual = scene->AddUI<ConsoleVisual>();
}

void GameClient::SendPlayerControl(){

    if(player->GetTransform()->position != previous_player_position){
        
        previous_player_position = player->GetTransform()->position;
        SendPacket<p_PlayerControl>(server, {{PACKET_PlayerControl, client_id}, player->GetComponent<SpriteRenderer>()->GetFlip(), previous_player_position.x, previous_player_position.y });
    }
}

void GameClient::SendSetBlock(short tile_index, int x, int y){
    SendPacket<p_SetBlock>(server, {{PACKET_SetBlock, client_id}, tile_index, x, y});
}

void GameClient::SendChatMessage(const std::string& message){
    return;
    char msg[50];
    SendPacket<p_ChatMessage>(server, {{PACKET_ChatMessage, client_id}});
}


void GameClient::Update(){

    // if the server hasn't responded in a while, timeout / drop connection
    if(time_since_last_packet > timeout_limit){
        Disconnect();
    }

    time_since_last_packet++;
}

void GameClient::CatchPeerEvent(ENetEvent event){

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

            world->SetTile(body.tile_index, body.pos_x, body.pos_y, SetLocation::FOREGROUND, SetMode::OVERRIDE, false);
            
            break;
        }

        case PACKET_ChatMessage : {
            
            p_ChatMessage body;
            memcpy(&body.message, event.packet->data, sizeof(p_ChatMessage));

            console_visual->Print({"message recieved"}, false);

            break;
        }
    }
}

void GameClient::OnDisconnect(){
    scene->DeleteObject(world);
}