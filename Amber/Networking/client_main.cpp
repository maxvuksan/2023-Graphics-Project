#include <enet/enet.h>
#include <iostream>

int main(){



    ENetHost* client;
    /*
        configuring the ENetHost for the client
        
        NULL - the address to host, since this is a client we aren't hosting anything
        1 outgoing connection (to the server)
        1 channel 
        0 incoming bandwidth (do not limit)
        0 outgoing bandwidth (do not limit)
    */
    client = enet_host_create(NULL, 1, 1, 0, 0);

    if(client == NULL){
        std::cout << "ERROR : Could not create an ENetHost for the client\n";
    }


    ENetAddress address; // will hold IP address and port for the server
    ENetEvent event; // the events sent from the server (mostly data)
    ENetPeer* peer; // the server we will be connecting to

    // configure where to contact the server
    enet_address_set_host(&address, "127.0.0.1");
    address.port= 7777;

    // connect the ENetHost client to the server located at 'address', 1 channel, 0 data to send initally
    peer = enet_host_connect(client, &address, 1, 0);
    
    if(peer == NULL){
        std::cout << "ERROR : Could not create ENet connection\n";
        return 1;
    }

    /* 
        enet_host_service() allows us to see any events from the server
        returns the number of events recieved,

        we initally check if the server sends back a successful connection event

        5000 number of milliseconds we want to wait between event checks
    */
    if(enet_host_service(client, &event, 5000) > 0 
        && event.type == ENET_EVENT_TYPE_CONNECT){
            std::cout << "Connection established\n";
        }
    else{
        enet_peer_reset(peer);
        std::cout << "ERROR : Connection failed\n";
        /*
            if the connection could not succed simply take a step back

            e.g. transfer the user back to the main menu, dont simply crash the program
        */
       return 0;
    }

    // game loop
    while(enet_host_service(client, &event, 1000) > 0){
        
        switch(event.type)
        {       
            // recieved data
            printf ("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                            event.packet -> dataLength,
                            event.packet -> data,
                            event.peer -> address.host,
                            event.peer->address.port,
                            event.channelID);
            break;
        }
    }

    // signal to the server we want to disconnect
    enet_peer_disconnect(peer, 0);

    // maybe not needed? 
    while(enet_host_service(client, &event, 3000) > 0){
        
        switch(event.type){

            case ENET_EVENT_TYPE_RECEIVE:
                // ignore any packets, we are just trying to disconnect
                enet_packet_destroy(event.packet);
                break;
            
            // server signals we have disconnected successfully
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected successfully\n";
                break;
        }
    }

    // keep watching ... https://www.youtube.com/watch?v=lWlZOTFW5lM

    return 0;
}