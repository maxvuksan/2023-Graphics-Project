#pragma once
#include "../../Amber/Framework.h"
#include "PacketTypes.h"

class GameClient : public Client {

    public:
        void SendStuff();
        void CatchPeerEvent(ENetEvent event) override;
};