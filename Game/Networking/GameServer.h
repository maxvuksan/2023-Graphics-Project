#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"
#include "../World.h"

class GameServer : public Server {

    public:
        GameServer();

        void Update() override;

        void CatchPeerEvent(ENetEvent event) override;

        void InterpretPacket(ENetEvent& event, PACKET packet_type);

        // broadcasts a packet to all clients
        template <typename T>
        void SendPacketToAll(T packet){
            
            for(auto& client : connected_clients){
                SendPacket<T>(client.second, packet);
            }
        }

        // broadcasts a packet to all clients but excludes the given client_id
        template <typename T>
        void SendPacketButExclude(T packet, int client_id){
            
            // sending new position to all clients besides the one which sent it
            for(auto& client : connected_clients){
                

                // dont send position back to originally sender
                if(client_id != client.first){
                    
                    SendPacket<T>(client.second, packet);
                }

            }
        }
        // send packet to specific client
        template <typename T>
        void SendPacketToSpecific(T packet, int client_id){
            SendPacket<T>(connected_clients[client_id], packet);
        }

        
        // similarly, passing packets without casting...
        void ForwardPacketToAll(ENetPacket* enet_packet);
        void ForwardPacketToSpecific(ENetPacket* enet_packet, int client_id);
        void ForwardPacketButExclude(ENetPacket* enet_packet, int client_id);

    private:

        // how often the server sends a heartbeat to the client
        int heartbeat_delay = 150;
        int heartbeat_delay_tracked;

        int client_id_tracked; // increments for every connection,
        std::map<int, ENetPeer*> connected_clients;
};