#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"
#include "../CommandParser.h"
#include "../Player/Inventory.h"
#include "../Pathfinding/PathfindingGraph.h"
#include "../Serilizer.h"
#include "../PlayMode.h"
#include <queue>
#include <vector>

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
        

        // how long the user should wait before asking for chunks again
        float chunk_retry_delay = 300;
        float chunk_retry_delay_tracked;
        std::vector<std::vector<bool>> chunks_success_grid; // what chunks have been successfully retrieved

        // a queue of clients that are requesting chunk data
        std::queue<int> players_to_transfer_chunks_to;
        // the current chunk coordinate being sent, is only relevant to the host (distrubuting chunks to the server)
        bool transfer_chunks = false;
        int current_transfer_client_id;
        int chunk_transfer_x;
        int chunk_transfer_y;

        

        bool world_loaded = false;


        PlayMode play_mode;

        bool allow_timeout = true;

        float timeout_limit = 1000;
        float time_since_last_packet;

        Scene* scene;

        int chunks_recieved = 0; // how many chunks this client has been sent, is used to determine if all chunks have been sent

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