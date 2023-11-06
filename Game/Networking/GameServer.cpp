#include "GameServer.h"

void GameServer::CatchPeerEvent(ENetEvent event){

    switch(event.type) 
    {   
        case ENET_EVENT_TYPE_CONNECT: {
            std::cout << "New connection " << event.peer->address.host << ":" << event.peer->address.port << "\n";
            break;
        }



        case ENET_EVENT_TYPE_RECEIVE: {

            // copy to the header portion of the message into PacketHeader object
            PacketHeader header;
            memcpy(&header, event.packet->data, sizeof(PacketHeader));

            InterpretPacket(event, (PACKET)header.type);

            enet_packet_destroy(event.packet);

            break;        
        }
        


        case ENET_EVENT_TYPE_DISCONNECT: {
            std::cout << event.peer->address.host << ":" << event.peer->address.port << " Disconnected\n";
            break;
        }

    }
}

void GameServer::InterpretPacket(ENetEvent& event, PACKET packet_type){
    
    switch(packet_type){

        case PACKET_PlayerControl: {
            
            PlayerControl body;
            memcpy(&body, event.packet->data, sizeof(PlayerControl));

            std::cout << "type: " <<body.header.type << "\n";
            std::cout << "x: " << body.pos_x << "\n";
            std::cout << "y: " << body.pos_y << "\n";
        }
    }
}