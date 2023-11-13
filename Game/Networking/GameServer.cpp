#include "GameServer.h"

GameServer::GameServer(): client_id_tracked(0) {}

void GameServer::Update(){
    if(heartbeat_delay_tracked > heartbeat_delay){
        heartbeat_delay_tracked = 0;
        SendPacketToAll<PacketHeader>({PACKET_ServerHeartbeat});
    }
    heartbeat_delay_tracked++;
}

void GameServer::CatchPeerEvent(ENetEvent event){

    switch(event.type) 
    {   
        case ENET_EVENT_TYPE_CONNECT: {

            if(Core::DEBUG_MODE){
                std::cout << "New connection " << event.peer->address.host << ":" << event.peer->address.port << "\n";
            }

            // giving the new client a new client_id
            SendPacket<PacketHeader>(event.peer, {PACKET_SetClientId, client_id_tracked});
            
            // define exisiting players on new player
            for(auto client : connected_clients){
                SendPacket<PacketHeader>(event.peer, {PACKET_CreatePlayer, client.first});
            }
            
            connected_clients[client_id_tracked] = event.peer;

            // define the new player on exisiting players
            SendPacketButExclude<PacketHeader>({PACKET_CreatePlayer, client_id_tracked}, client_id_tracked);

     
            // increment client_id counter
            client_id_tracked++;
            
            break;
        }



        case ENET_EVENT_TYPE_RECEIVE: {

            PacketHeader header;
            memcpy(&header, event.packet->data, sizeof(PacketHeader));

            InterpretPacket(event, (PACKET)header.type);

            enet_packet_destroy(event.packet);

            break;        
        }
        


        case ENET_EVENT_TYPE_DISCONNECT: {
            std::cout << event.peer->address.host << ":" << event.peer->address.port << " Disconnected\n";

            // get client id of disconnection
            int disconnecting_id = 0;
            for(auto client : connected_clients){
                if(client.second == event.peer){
                    disconnecting_id = client.first;
                }
            }

            connected_clients.erase(disconnecting_id);

            // define the new player on exisiting players
            SendPacketToAll<PacketHeader>({PACKET_CreatePlayer, disconnecting_id});
     
            break;
        }

    }
}

void GameServer::InterpretPacket(ENetEvent& event, PACKET packet_type){

    PacketHeader header;
    memcpy(&header, event.packet->data, sizeof(PacketHeader));

    switch(packet_type){

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
        ForwardPacket(client.second, enet_packet);
    }
}

void GameServer::ForwardPacketButExclude(ENetPacket* enet_packet, int client_id){
    
    // sending new position to all clients besides the one which sent it
    for(auto& client : connected_clients){
        
        // dont send position back to originally sender
        if(client_id != client.first){
            
            ForwardPacket(client.second, enet_packet);
        }
    }
}

void GameServer::ForwardPacketToSpecific(ENetPacket* enet_packet, int client_id){
    ForwardPacket(connected_clients[client_id], enet_packet);
}