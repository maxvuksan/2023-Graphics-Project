#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"
#include "../CommandParser.h"
#include "../Player/Inventory.h"
#include "../Pathfinding/PathfindingGraph.h"
#include "../Serilizer.h"
#include "../PlayMode.h"
#include <queue>
#include <string>
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

        Player* GetPlayer(){return player;}

        // can the client timeout from the server?
        void SetAllowTimeout(bool allow_timeout);

        void SendPlayerControl();
        void SendSetBlock(signed_byte tile_index, int x, int y, SetLocation set_location);
        void SendChatMessage(const std::string& message);

        void CatchPeerEvent(ENetEvent event) override;
        void InterpretPacket(ENetEvent& event);


        void OnDisconnect() override;

        void SetPlayMode(PlayMode play_mode);
        PlayMode GetPlayMode(){return play_mode;}
        
        Inventory* GetInventory(){return inventory;}

        static sf::Vector2f player_pos;

        // @returns a map of all the clients connected to the server
        const std::map<int, Player*>& GetConnectedClients(){return connected_clients;}

        void SetTargetPort(int port){target_port = port;}
        int GetTargetPort(){return target_port;}
        void SetTargetIPAddress(const std::string& ip){target_ip_address = ip;}
        const std::string& GetTargetIPAddress(){return target_ip_address;}

    private:
        

        // how long the user should wait before asking for chunks again
        float chunk_retry_delay = 800;
        float chunk_retry_delay_tracked;
        std::vector<std::vector<bool>> chunks_success_grid; // what chunks have been successfully retrieved

        // a queue of clients that are requesting chunk data
        std::queue<int> players_to_transfer_chunks_to;
        // the current chunk coordinate being sent, is only relevant to the host (distrubuting chunks to the server)
        bool transfer_chunks = false;
        int current_transfer_client_id;
        int chunk_transfer_x;
        int chunk_transfer_y;

        
        // has this client recieved all chunk data, world header etc...
        bool world_loaded = false;
        int chunks_recieved = 0; // how many chunks this client has been sent, is used to determine if all chunks have been sent

        PlayMode play_mode; // are we online? hosting or joining someone

        bool allow_timeout = true; // can a connection timeout?
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

        // what IP are we connecting to ?
        std::string target_ip_address;
        int target_port;

};