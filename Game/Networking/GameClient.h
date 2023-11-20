#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"
#include "../CommandParser.h"
#include "../Player/Inventory.h"
#include "../Pathfinding/PathfindingGraph.h"

class World;
class Player;
class PlayerController;
class ConsoleVisual;
class PlayerWorldInteractions;

class GameClient : public Client {

    friend class CommandParser; 

    public:

        void LinkScene(Scene* scene);

        void Update() override;

        void CreateObjects();

        void SendPlayerControl();
        void SendSetBlock(short tile_index, int x, int y);
        void SendChatMessage(const std::string& message);

        void CatchPeerEvent(ENetEvent event) override;
        void InterpretPacket(ENetEvent& event);

        void OnDisconnect() override;

        Inventory* GetInventory(){return inventory;}

    private:
        
        int timeout_limit = 300;
        int time_since_last_packet;

        bool playing_online = false;

        Scene* scene;

        int client_id = -1;
        std::map<int, Player*> connected_clients;

        Player* player;
        PlayerController* player_controller;
        Inventory* inventory;
        PlayerWorldInteractions* player_world_interactions;

        // used to determine if we should send a p_PlayerControl to the server
        sf::Vector2f previous_player_position;

        ConsoleVisual* console_visual;
        World* world;

        PathfindingGraph* pathfinding_graph;
};