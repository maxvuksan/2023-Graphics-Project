#pragma once
#include "../../Amber/Framework.h"

#define PACKET_STRING_SIZE 50

enum PACKET{
    
    /*
        In the case the client is not recieving any packets, to determine if the server is still alive, 
        we communicate a heartbeat to all clients, by tracking this heartbeat on the client side we can determine if the server is still alive
    */
    PACKET_ServerHeartbeat,
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
    
    PACKET_SetChunk,
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
    float pos_x;
    float pos_y;
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

struct p_SetChunk{
    PacketHeader header;
    bool target_client_id; // who should recieve this chunk?
    short foreground_grid[16][16];
    short background_grid[16][16];
};
