#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"
#include "../World/World.h"




// infomation that server holds about a client
struct ClientData{
    ENetPeer* peer;
    bool allow_timeout = true;
    int time_since_last_packet = 0; // how long has it been since we've heard from this client
    
    std::vector<ENetPacket*> held_packets; // some events must wait for the world to be loaded, thus we hold this packets and send them later
    bool loading_world = false;
};

class GameServer : public Server {

    public:
        GameServer();

        void Update() override;

        void DisconnectClient(int client_id);

        void CatchPeerEvent(ENetEvent event) override;

        void InterpretPacket(ENetEvent& event, PACKET packet_type);

        // broadcasts a packet to all clients
        template <typename T>
        void SendPacketToAll(T packet){
            
            for(auto& client : connected_clients){
                SendPacket<T>(client.second.peer, packet);
            }
        }

        // broadcasts a packet to all clients but excludes the given client_id
        template <typename T>
        void SendPacketButExclude(T packet, int client_id){
            
            // sending new position to all clients besides the one which sent it
            for(auto& client : connected_clients){
                

                // dont send position back to originally sender
                if(client_id != client.first){
                    
                    SendPacket<T>(client.second.peer, packet);
                }

            }
        }
        // send packet to specific client
        template <typename T>
        void SendPacketToSpecific(T packet, int client_id){
            SendPacket<T>(connected_clients[client_id].peer, packet);
        }

        // similarly, passing packets without casting...
        void ForwardPacketToAll(ENetPacket* enet_packet);
        void ForwardPacketToSpecific(ENetPacket* enet_packet, int client_id);
        void ForwardPacketButExclude(ENetPacket* enet_packet, int client_id);

        void SetWorld(World* world){this->world = world;}
        World* GetWorld(){return world;}

    private:

    

        // how often the server sends a heartbeat to the client
        float heartbeat_delay = 30;
        float heartbeat_delay_tracked;

        int timeout_time = 1000;

        int client_id_tracked; // increments for every connection,
        std::map<int, ClientData> connected_clients;

        World* world;
};