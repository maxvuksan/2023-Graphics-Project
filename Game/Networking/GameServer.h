#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"

class GameServer : public Server {

    public:
        void CatchPeerEvent(ENetEvent event) override;

        void InterpretPacket(ENetEvent& event, PACKET packet_type);
};