#pragma once
#include "NetworkObject.h"

/*
    A client abstraction upon the enet networking libary
*/
class Client : public NetworkObject{

    public:

        Client();

        /*
            @returns true if a connection was successfully established, false otherwise
        */
        bool Connect(const char* address_str, enet_uint16 port);
        void Disconnect();

        virtual void OnDisconnect(){};

        ~Client() override;

    protected:

        ENetPeer* server; 
};