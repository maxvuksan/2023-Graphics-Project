#pragma once
#include "../../Amber/Framework.h"

enum PACKET{
    PACKET_PlayerControl,
};

/*
    All packets are required to start with packet header
*/

struct PlayerControl{

    PacketHeader header;
    float pos_x;
    float pos_y;
};