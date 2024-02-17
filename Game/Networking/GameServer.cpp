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

            std::cout << "[SERVER]: new connection" << event.peer->address.host << ":" << event.peer->address.port << "\n";

            // giving the new client a new client_id
            SendPacket<PacketHeader>(event.peer, {PACKET_SetClientId, client_id_tracked});
            
            // define exisiting players on new player
            for(auto client : connected_clients){
                SendPacket<PacketHeader>(event.peer, {PACKET_CreatePlayer, client.first});
            }
            
            connected_clients[client_id_tracked].client_id = client_id_tracked;
            connected_clients[client_id_tracked].peer = event.peer;
            connected_clients[client_id_tracked].time_since_last_packet = 0;

            // define the new player on exisiting players
            SendPacketButExclude<PacketHeader>({PACKET_CreatePlayer, client_id_tracked}, client_id_tracked);

            // we are not the host
            if(client_id_tracked != 0){
                connected_clients[client_id_tracked].loading_world = true;
                // ask the host for the world header
                SendPacketToSpecific<PacketHeader>({PACKET_RequestWorldHeader, client_id_tracked}, 0);
            }

            // increment client_id counter
            client_id_tracked++;
            
            std::cout << "[SERVER]: connection complete\n";

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

            std::cout << "[SERVER]: disconnected " << event.peer->address.host << ":" << event.peer->address.port;

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
            break;
        }

        case PACKET_WorldHeader:{
    
            p_WorldHeader body;
            memcpy(&body, event.packet->data, sizeof(p_WorldHeader));
            
            std::cout << "[SERVER]: sending PACKET_WorldHeader to client" << body.target_client_id << "\n";

            ForwardPacketToSpecific(event.packet, body.target_client_id);  
            break;
        }   
        case PACKET_RequestChunks: {
            
            // request chunks from host
            ForwardPacketToSpecific(event.packet, 0);  
            break;
        }
        case PACKET_SetChunk: {


            p_SetChunk body;
            memcpy(&body, event.packet->data, sizeof(p_WorldHeader));

            std::cout << "[SERVER]: sending chunk " << body.chunk_coordinate_x << ", " << body.chunk_coordinate_y << " to client " << body.target_client_id << "\n";

            ForwardPacketToSpecific(event.packet, body.target_client_id);
            break;
        }
        case PACKET_RequestSpecificChunk: {
            // forward request to host
            ForwardPacketToSpecific(event.packet, 0);
            break;
        }
        case PACKET_WorldLoadedSuccessfully: {
            connected_clients[header.client_id].loading_world = false;

            std::cout << "[SERVER]: client " << header.client_id << " loaded world successfully, sending all queued packets\n";

            // forward all queued packets to the client
            for(int i = 0; i < connected_clients[header.client_id].held_packets.size(); i++){
                ForwardPacketToSpecific(connected_clients[header.client_id].held_packets[i], header.client_id);
            }
            connected_clients[header.client_id].held_packets.clear();
            break;
        }


        case PACKET_PlayerControl: {
            ForwardPacketButExclude(event.packet, header.client_id, DISCARD);  
            break;
        }
        case PACKET_SetBlock: {
            std::cout << "[SERVER]: PACKET_SetBlock broadcasted by client " << header.client_id << "\n";

            ForwardPacketButExclude(event.packet, header.client_id, QUEUE);  
            break;
        }
        case PACKET_ChatMessage:       
            ForwardPacketButExclude(event.packet, header.client_id, DISCARD);  
            break;

        
        
    }
}


 // broadcasts a packet to all clients
void GameServer::ForwardPacketToAll(ENetPacket* enet_packet, PacketLoadingMode packet_loading_mode){
    
    for(auto& client : connected_clients){
        ForwardPacketToSpecific(enet_packet, client.first, packet_loading_mode);
    }
}

void GameServer::ForwardPacketButExclude(ENetPacket* enet_packet, int client_id, PacketLoadingMode packet_loading_mode){
    
    // sending new position to all clients besides the one which sent it
    for(auto& client : connected_clients){
        
        // dont send position back to originally sender
        if(client_id != client.first){
            
            ForwardPacketToSpecific(enet_packet, client_id, packet_loading_mode);
        }
    }
}

void GameServer::ForwardPacketToSpecific(ENetPacket* enet_packet, int client_id, PacketLoadingMode packet_loading_mode){

    if(!ClientIdExists(client_id)){
        return;
    }

    if(connected_clients[client_id].loading_world){

        switch(packet_loading_mode){

            case SEND_ANYWAY:{
                ForwardPacket(connected_clients[client_id].peer, enet_packet);
                break;
            }
            case QUEUE: {
                connected_clients[client_id].held_packets.push_back(enet_packet);
                break;
            }
            case DISCARD:{
                // do nothing, lose the packet...
                break;
            }

        }

    }
    else{
        ForwardPacket(connected_clients[client_id].peer, enet_packet);
    }

}

bool GameServer::ClientIdExists(int client_id){
    for(int i = 0; i < connected_clients.size(); i++){
        if(connected_clients[i].client_id == client_id){
            return true;
        }
    }
    return false;
}