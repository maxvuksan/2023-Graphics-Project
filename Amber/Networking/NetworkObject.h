#include <enet/enet.h>
#include <pthread.h>
/*
    class abstraction on top of the enet networking libary 

    enet libary : http://enet.bespin.org/Tutorial.html
*/
class NetworkObject{

    public:
        NetworkObject(): thread_running(false) {}
        virtual ~NetworkObject(){}

    protected:

        // to allow pthread to workin classes, we provide a static entry function which funnels to the object instance function
        static void* ListenerThread_Entry(void* _this);
        void ListenerThread();

        virtual void CatchPeerEvent(ENetEventType event_type){}

        void SendPacket(ENetPeer* peer, const char* data);

        // terminates the listener thread loop, is called when we want to end the connection
        void ListenerClose();

        ENetHost* client;
        ENetAddress address;
        ENetEvent event;

        pthread_t listener_thread;
        pthread_mutex_t listener_thread_lock;
        bool thread_running;
};