#include "Client.h"
#include <iostream>

Client::Client(){

    if(enet_initialize() != 0){
        std::cout << "ERROR : Could not initialize enet\n";
        return;
    }
    atexit(enet_deinitialize);

    client = enet_host_create(NULL, 1, 1, 0, 0);

    if(client == NULL){
        std::cout << "ERROR : Could not create an ENetHost for the client, Client::Client()\n";
        return;
    }
}


bool Client::Connect(const char* address_str, enet_uint16 port){

    // attempting connection...

    // configure where to contact the server
    enet_address_set_host(&address, address_str);
    address.port = port;

    peer = enet_host_connect(client, &address, 1, 0);
    
    if(peer == NULL){
        std::cout << "ERROR : Could not create ENet connection, Client::Connect()\n";
        return false;
    }

    // waiting for servers response to connection...

    if(enet_host_service(client, &event, 5000) > 0 
        && event.type == ENET_EVENT_TYPE_CONNECT){
            return true;
        }

    else{
        enet_peer_reset(peer);

        std::cout << "ERROR : Connection failed, Client::Connect()\n";

        return false;
    }
}

void Client::Disconnect(){
    // signal to the server we want to disconnect
    enet_peer_disconnect(peer, 0);
}

Client::~Client(){
    Disconnect();
}