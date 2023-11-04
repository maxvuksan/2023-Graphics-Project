#include "NetworkObject.h"

/*
    A server abstraction upon the enet networking libary
*/
class Server : public NetworkObject {

    public:

        Server();

        void Run(enet_uint16 port, size_t max_clients = 16);
        void Close();

        ~Server() override;

    protected:
     
        void CatchPeerEvent(ENetEventType event_type) override;

};