#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"

class World;
class Player;
class GameClient : public Client {

    public:

        void LinkScene(Scene* scene);

        void Update() override;

        // create the player and the world
        void CreateObjects();

        void SendPlayerControl();
        void SendSetBlock(short tile_index, int x, int y);

        void CatchPeerEvent(ENetEvent event) override;
        void InterpretPacket(ENetEvent& event);

        void OnDisconnect() override;

    private:
        
        int timeout_limit = 300;
        int time_since_last_packet;

        Scene* scene;

        int client_id = -1;
        std::map<int, Player*> connected_clients;

        Player* player;
        sf::Vector2f previous_player_position;

        World* world;

};