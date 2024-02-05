#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"
#include "../CommandParser.h"
#include "../Player/Inventory.h"
#include "../Pathfinding/PathfindingGraph.h"
#include "../Serilizer.h"
#include "../PlayMode.h"

class World;
class Player;
class PlayerController;
class ConsoleVisual;
class PlayerWorldInteractions;
class HealthBar;

class GameClient : public Client {

    friend class CommandParser; 

    public:

        void LinkScene(Scene* scene);
        void LinkWorld(World* world);

        void Update() override;

        // is called when the player firsts joins, creates player object and components ... etc
        void CreateObjects();

        /*
            both current_player and curren_world are Serilizer DataPair objects

            holding infomation about 
            1. what is the name of the current player?
            2. what is the filepath they should be saved it
        */
        void SetCurrentPlayer(Serilizer::DataPair player);
        void SetCurrentWorld(Serilizer::DataPair world);
        const Serilizer::DataPair& GetCurrentPlayer() const{ return current_player;}
        const Serilizer::DataPair& GetCurrentWorld() const { return current_world;}


        // can the client timeout from the server?
        void SetAllowTimeout(bool allow_timeout);

        void SendPlayerControl();
        void SendSetBlock(short tile_index, int x, int y);
        void SendChatMessage(const std::string& message);

        void CatchPeerEvent(ENetEvent event) override;
        void InterpretPacket(ENetEvent& event);


        void OnDisconnect() override;

        void SetPlayMode(PlayMode play_mode);
        PlayMode GetPlayMode(){return play_mode;}
        
        Inventory* GetInventory(){return inventory;}

        static sf::Vector2f player_pos;

    private:
        
        PlayMode play_mode;

        bool allow_timeout = true;

        float timeout_limit = 1000;
        float time_since_last_packet;

        Scene* scene;

        int client_id = -1;
        std::map<int, Player*> connected_clients;

        Player* player;
        PlayerController* player_controller;
        HealthBar* health_bar;
        Inventory* inventory;
        PlayerWorldInteractions* player_world_interactions;

        // used to determine if we should send a p_PlayerControl to the server
        sf::Vector2f previous_player_position;

        ConsoleVisual* console_visual;
        World* world;

        Serilizer::DataPair current_player;
        Serilizer::DataPair current_world;

        PathfindingGraph* pathfinding_graph;

};