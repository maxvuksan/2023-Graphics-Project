#include "GameClient.h"

void GameClient::SendStuff()
{
    SendPacket<PlayerControl>(server, {PACKET_PlayerControl, 10, 15});
}

void GameClient::CatchPeerEvent(ENetEvent event){
    
}