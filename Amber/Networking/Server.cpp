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


void Server::Close(){
    
    ListenerClose();
    enet_host_destroy(client);
}



Server::~Server(){

    Close();
    enet_deinitialize();
    
}