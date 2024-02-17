#pragma once
#include "../../Amber/Framework.h"
#include <array>

#define PACKET_STRING_SIZE 50

enum PACKET{
    
    /*
        In the case the client is not recieving any packets, to determine if the server is still alive, 
        we communicate a heartbeat to all clients, by tracking this heartbeat on the client side we can determine if the server is still alive
    */
    PACKET_ServerHeartbeat,
    // allows expensive operations, like loading the world pause timing out
    PACKET_DisableTimeout,
    PACKET_EnableTimeout,
    /*
        When a client first joins the server they are assigned a client_id
    */
    PACKET_SetClientId,
    PACKET_CreatePlayer,
    PACKET_DeletePlayer,

    /*
        When a clients position and or relavant data changes
    */
    PACKET_PlayerControl,
    PACKET_SetBlock,
    PACKET_ChatMessage,
    
    /*
        We are asking a client to send the world header
    */
    PACKET_RequestWorldHeader,
    PACKET_WorldHeader,
    /*
        when we have recieved the world header we request each individual chunk
    */
    PACKET_RequestChunks, // the client is ready to be sent chunks
    PACKET_SetChunk,
    PACKET_RequestSpecificChunk,
    PACKET_WorldLoadedSuccessfully,

};

struct PacketHeader{
    uint8_t type; // type of the message
    int client_id = -1; // index of client, if the client_id is not important (e.g. originated from the server, we can ignore it)
};

/*
    All packets are required to start with packet header
*/


struct p_PlayerControl{

    PacketHeader header;
    bool flip_sprite;
    int animation_state_index;
    float pos_x;
    float pos_y;
    float velocity_x;
    float velocity_y;
};

struct p_SetBlock{
    PacketHeader header;
    short tile_index;
    int pos_x;
    int pos_y;
};

struct p_ChatMessage{
    PacketHeader header;
    char message;
};

struct p_WorldHeader{

    PacketHeader header;
    int target_client_id; // who is this world header for

    int spawnpoint_x;
    int spawnpoint_y;

    // in chunks
    int width;
    int height;
};

struct p_SetChunk{
    PacketHeader header;
    int target_client_id; // who is this chunk for

    // location of chunk
    int chunk_coordinate_x;
    int chunk_coordinate_y;
    
    std::array<std::array<signed_byte, TILEMAP_HEIGHT>, TILEMAP_WIDTH> main_grid;
    std::array<std::array<signed_byte, TILEMAP_HEIGHT>, TILEMAP_WIDTH> background_grid;
    std::array<std::array<signed_byte, TILEMAP_HEIGHT>, TILEMAP_WIDTH> foreground_grid;
};


// server should respond with a p_SetChunk packet 
struct p_RequestSpecificChunk{

    PacketHeader header;
    // location of chunk
    int chunk_coordinate_x;
    int chunk_coordinate_y;
};




