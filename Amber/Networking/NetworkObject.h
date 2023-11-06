#pragma once
#include <enet/enet.h>
#include <pthread.h>
#include "PacketHeader.h"
#include <iostream>
#include "../Utility/Memory.h"
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

        virtual void CatchPeerEvent(ENetEvent event){}

        /*
            the header_type enum should be relateted to the type T we are sending
        */
        template <typename T>
        void SendPacket(ENetPeer* peer, T packet){

            ENetPacket* enet_packet = enet_packet_create((void*)&packet, sizeof(packet), ENET_PACKET_FLAG_RELIABLE);
    
            enet_peer_send(peer, 0, enet_packet);
    
        }

        // terminates the listener thread loop, is called when we want to end the connection
        void ListenerClose();

        ENetHost* client;
        ENetAddress address;

        pthread_t listener_thread;
        pthread_mutex_t listener_thread_lock;
        bool thread_running;
};