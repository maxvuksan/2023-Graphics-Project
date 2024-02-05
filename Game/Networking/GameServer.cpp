#include "GameServer.h"

GameServer::GameServer(): client_id_tracked(0) {}

void GameServer::Update(){
    if(heartbeat_delay_tracked > heartbeat_delay){
        heartbeat_delay_tracked = 0;
        SendPacketToAll<PacketHeader>({PACKET_ServerHeartbeat});
    }
    heartbeat_delay_tracked++;



    for(auto& client : connected_clients){

        if(!client.second.allow_timeout){
            continue;
        }

        client.second.time_since_last_packet++;

        if(client.second.time_since_last_packet > timeout_time){

            DisconnectClient(client.first);
        }
    }
}

void GameServer::DisconnectClient(int disconnecting_id){
    connected_clients.erase(disconnecting_id);

    // define the new player on exisiting players
    SendPacketToAll<PacketHeader>({PACKET_DeletePlayer, disconnecting_id});
}

void GameServer::CatchPeerEvent(ENetEvent event){

    switch(event.type) 
    {   
        case ENET_EVENT_TYPE_CONNECT: {

            std::cout << "New connection " << event.peer->address.host << ":" << event.peer->address.port << "\n";

            // giving the new client a new client_id
            SendPacket<PacketHeader>(event.peer, {PACKET_SetClientId, client_id_tracked});
            
            // define exisiting players on new player
            for(auto client : connected_clients){
                SendPacket<PacketHeader>(event.peer, {PACKET_CreatePlayer, client.first});
            }
            
            connected_clients[client_id_tracked].peer = event.peer;
            connected_clients[client_id_tracked].time_since_last_packet = 0;

            // define the new player on exisiting players
            SendPacketButExclude<PacketHeader>({PACKET_CreatePlayer, client_id_tracked}, client_id_tracked);

            std::cout << "Connection Complete\n";

            if(client_id_tracked != 0){
                connected_clients[client_id_tracked].loading_world = true;
                // ask the host world to world header
                SendPacketToSpecific<PacketHeader>({PACKET_RequestWorldHeader, client_id_tracked}, 0);
            }

            // increment client_id counter
            client_id_tracked++;
            
            break;
        }



        case ENET_EVENT_TYPE_RECEIVE: {

            PacketHeader header;
            memcpy(&header, event.packet->data, sizeof(PacketHeader));

            // check if the client sending a packet exists on the server,
            // if it does not it may have been kicked for some reason
            bool client_exists = false;
            for(auto& client : connected_clients){
                if(client.first == header.client_id){
                    client_exists = true;
                }
            }

            if(!client_exists){
                break;
            }

            // mark clients activity, prevent timeout
            connected_clients[header.client_id].time_since_last_packet = 0;

            InterpretPacket(event, (PACKET)header.type);

            enet_packet_destroy(event.packet);

            break;        
        }
        


        case ENET_EVENT_TYPE_DISCONNECT: {
            std::cout << event.peer->address.host << ":" << event.peer->address.port << " Disconnected\n";

            // get client id of disconnection
            int disconnecting_id = 0;
            for(auto client : connected_clients){
                if(client.second.peer == event.peer){
                    disconnecting_id = client.first;
                }
            }

            DisconnectClient(disconnecting_id);
     
            break;
        }

    }
}

void GameServer::InterpretPacket(ENetEvent& event, PACKET packet_type){

    PacketHeader header;
    memcpy(&header, event.packet->data, sizeof(PacketHeader));

    switch(packet_type){

        case PACKET_EnableTimeout: {
            connected_clients[header.client_id].allow_timeout = true;
            break;
        }
        case PACKET_DisableTimeout: {
            connected_clients[header.client_id].allow_timeout = false;
        }

        case PACKET_WorldHeader:{
    
            p_WorldHeader body;
            memcpy(&body, event.packet->data, sizeof(p_WorldHeader));
            
            ForwardPacketToSpecific(event.packet, body.target_client_id);  
        }


        case PACKET_PlayerControl: 
        case PACKET_SetBlock: 
        case PACKET_ChatMessage:       
            ForwardPacketButExclude(event.packet, header.client_id);  
            break;
        
    }
}


 // broadcasts a packet to all clients
void GameServer::ForwardPacketToAll(ENetPacket* enet_packet){
    
    for(auto& client : connected_clients){
        ForwardPacket(client.second.peer, enet_packet);
    }
}

void GameServer::ForwardPacketButExclude(ENetPacket* enet_packet, int client_id){
    
    // sending new position to all clients besides the one which sent it
    for(auto& client : connected_clients){
        
        // dont send position back to originally sender
        if(client_id != client.first){
            
            ForwardPacket(client.second.peer, enet_packet);
        }
    }
}

void GameServer::ForwardPacketToSpecific(ENetPacket* enet_packet, int client_id){
    ForwardPacket(connected_clients[client_id].peer, enet_packet);
}