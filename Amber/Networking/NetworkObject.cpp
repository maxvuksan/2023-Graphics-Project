#include "NetworkObject.h"

void* NetworkObject::ListenerThread_Entry(void* _this){
    ((NetworkObject*)(_this))->ListenerThread();
    return NULL;
}

void NetworkObject::ForwardPacket(ENetPeer* peer, ENetPacket* enet_packet){
    enet_peer_send(peer, 0, enet_packet);
}


void NetworkObject::ListenerThread(){

    ENetEvent event;

    while(true){

        while(enet_host_service(client, &event, 15) > 0){
            
            // funnels the event...
            CatchPeerEvent(event);
        }
        
        Update();

        // check if we should terminate thread
        pthread_mutex_lock(&listener_thread_lock);
        
        if(!thread_running){
            break;
        }

        pthread_mutex_unlock(&listener_thread_lock);
    }
}

void NetworkObject::ListenerClose(){

    if(thread_running){
        
        // mark thread as finished
        pthread_mutex_lock(&listener_thread_lock);
        thread_running = false;
        pthread_mutex_unlock(&listener_thread_lock);

        // join thread
        pthread_join(listener_thread, NULL);

        // destroy mutex
        pthread_mutex_destroy(&listener_thread_lock);
    }
}