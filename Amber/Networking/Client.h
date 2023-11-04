#include <enet/enet.h>

/*
    A class abstraction built upon the enet networking libary
*/
class Client {

    public:

        Client();

        /*
            @returns true if a connection was successfully established, false otherwise
        */
        bool Connect(const char* address_str, enet_uint16 port);
        void Disconnect();

        void SendPacket(ENetPeer* peer, const char* data);
        
        ~Client();

    private:
        ENetHost* client;
        ENetPeer* peer; // the server in the clients case    

        ENetAddress address;
        ENetEvent event;
};