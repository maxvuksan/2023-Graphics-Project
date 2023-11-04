#include <enet/enet.h>
#include <iostream>

int main(){

    if(enet_initialize() != 0){
        std::cout << "ERROR : Could not initialize enet\n";
        return 1;
    }
    atexit(enet_deinitialize);

    int max_clients = 16;
    ENetHost* server;
    ENetAddress address;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = 7777;

    server = enet_host_create(&address, max_clients, 1, 0, 0);

    if(server == NULL){
        std::cout << "ERROR : Could not create an ENetHost for the server\n";
        return 1;
    }

    // game loop
    while(true){

        while(enet_host_service(server, &event, 1000) > 0){
        
            switch(event.type)
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
    }

    enet_host_destroy(server);

    return 0;
}