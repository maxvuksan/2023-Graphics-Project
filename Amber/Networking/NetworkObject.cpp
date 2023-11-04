#include "NetworkObject.h"

void* NetworkObject::ListenerThread_Entry(void* _this){
            ((NetworkObject*)(_this))->ListenerThread();
            return NULL;
}

void NetworkObject::SendPacket(ENetPeer* peer, const char* data){
    
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    
    /*
        arguments...

        peer to send to 
        channel index to send to 
        packet to send
    */
    enet_peer_send(peer, 0, packet);
}

void NetworkObject::ListenerThread(){

    while(true){

        while(enet_host_service(client, &event, 1000) > 0){
            
            // funnels the event...
            CatchPeerEvent(event.type);
        }

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