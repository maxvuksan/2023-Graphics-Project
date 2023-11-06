#pragma once
#include <iostream>

struct PacketHeader{
    uint8_t type; // type of the message
};

struct BodyTest{
    PacketHeader header;
    int a;
    int b;
    int c;
};