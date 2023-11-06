#include "Client.h"
#include <iostream>

Client::Client(){

    if(enet_initialize() != 0){
        std::cout << "ERROR : Could not initialize enet\n";
        return;
    }
    atexit(enet_deinitialize);

}

bool Client::Connect(const char* address_str, enet_uint16 port){

    // attempting connection...

    client = enet_host_create(NULL, 1, 1, 0, 0);

    if(client == NULL){
        std::cout << "ERROR : Could not create an ENetHost for the client, Client::Client()\n";
        return false;
    }

    // configure where to contact the server
    enet_address_set_host(&address, address_str);
    address.port = port;

    server = enet_host_connect(client, &address, 1, 0);
    
    if(server == NULL){
        std::cout << "ERROR : Could not create ENet connection, Client::Connect()\n";
        return false;
    }

    // waiting for server response to connection...

    ENetEvent event;
    if(enet_host_service(client, &event, 5000) > 0 
        && event.type == ENET_EVENT_TYPE_CONNECT){

            std::cout << "connected successfully\n";

        }

    else{
        enet_peer_reset(server);

        std::cout << "Connection failed\n";
    }

    pthread_mutex_init(&listener_thread_lock, NULL);
    pthread_create(&listener_thread, NULL, ListenerThread_Entry, this);
    thread_running = true;

    return true;

}

void Client::CatchPeerEvent(ENetEvent event){
    
    switch(event.type)
    {   
        case ENET_EVENT_TYPE_RECEIVE:
            printf ("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                            event.packet -> dataLength,
                            event.packet -> data,
                            event.peer -> address.host,
                            event.peer->address.port,
                            event.channelID);
            break;               
    }
}


void Client::Disconnect(){

    ListenerClose();

    // signal to the server we want to disconnect
    enet_peer_disconnect(server, 0);

    ENetEvent event;
    // wait for server to confirm successful disconnection
    while (enet_host_service (client, &event, 20) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected Successfully\n";
                return;
            
            // destroy any packets which aren't TYPE_DISCONNECT
            default:
                enet_packet_destroy (event.packet);
                break;

        }
    }

    enet_peer_reset(server);
}

Client::~Client(){
    Disconnect();
    enet_deinitialize();
}