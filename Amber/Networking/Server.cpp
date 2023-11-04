#include "Server.h"
#include <iostream>

Server::Server(){

    if(enet_initialize() != 0){
        std::cout << "ERROR : Could not initialize enet\n";
    }
    atexit(enet_deinitialize);
}

void Server::Run(enet_uint16 port, size_t max_clients){
    
    if(thread_running){
        std::cout << "ERROR : Server already running, please call Server::Close() before calling Server::Run() again\n";
    }

    address.host = ENET_HOST_ANY;
    address.port = port;

    client = enet_host_create(&address, max_clients, 1, 0, 0);

    if(client == NULL){
        std::cout << "ERROR : Could not create an ENetHost for the server\n";
    }

    pthread_mutex_init(&listener_thread_lock, NULL);
    pthread_create(&listener_thread, NULL, ListenerThread_Entry, this);
    thread_running = true;
}

void Server::CatchPeerEvent(ENetEventType event_type){

    switch(event_type)
    {   
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "New connection " << event.peer->address.host << ":" << event.peer->address.port << "\n";
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            printf ("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                            event.packet -> dataLength,
                            event.packet -> data,
                            event.peer -> address.host,
                            event.peer->address.port,
                            event.channelID);
            break;               
        
        case ENET_EVENT_TYPE_DISCONNECT:
            std::cout << event.peer->address.host << ":" << event.peer->address.port << " Disconnected\n";
            break;

    }
}

void Server::Close(){
    
    ListenerClose();
    enet_host_destroy(client);
}


Server::~Server(){

    Close();
    enet_deinitialize();
    
}