#include <enet/enet.h>

/*
    A server abstraction built upon the enet networking libary
*/
class Server{

    public:

        Server();

        /*

        */
        void Run(enet_uint16 port, enet_uint16 max_clients);


    private:
        ENetHost* server;
        ENetAddress address;
        ENetEvent event;

        
};