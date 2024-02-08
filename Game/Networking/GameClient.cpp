#include "GameClient.h"
#include "../World/World.h"
#include "../Player/PlayerController.h"
#include "../Player/Player.h"
#include "../ConsoleVisual.h"
#include "../Player/PlayerWorldInteractions.h"
#include "../Pathfinding/Fly.h"
#include "../Pathfinding/NoodleCreature.h"
#include "../Player/HealthBar.h"
#include "../../Amber/Framework.h"

sf::Vector2f GameClient::player_pos;

/*
    blocks multiplayer based code, when playing in single player
*/
#define IF_ONLINE if(play_mode == PlayMode::OFFLINE){return;}

void GameClient::LinkScene(Scene* scene){
    this->scene = scene;
}
void GameClient::LinkWorld(World* world){
    this->world = world;
}

void GameClient::CreateObjects(){
    

    // create world
    world->LinkClient(this);

    // pathfinding 
    //pathfinding_graph = scene->AddObject<PathfindingGraph>();
    //pathfinding_graph->LinkWorld(world);

    // setting scene bounds
    WorldProfile* wp = world->GetWorldProfile();
    scene->SetMinXBound(0);
    scene->SetMinYBound(0);
    scene->SetMaxXBound(wp->width * wp->tilemap_profile.width * wp->tilemap_profile.tile_width);
    scene->SetMaxYBound(wp->height * wp->tilemap_profile.height * wp->tilemap_profile.tile_height);

    // creating player and adding all player based components
    HealthBar* health_bar = scene->AddUI<HealthBar>();
    health_bar->SetCycle(TimeManager::GetTimeOfDay());
    health_bar->SetMaxCycle(TimeManager::GetTotalTimeInDay());

    player = scene->AddObject<Player>();
    player_controller = player->AddComponent<PlayerController>();

    player_world_interactions = player->AddComponent<PlayerWorldInteractions>();
    inventory = scene->AddUI<Inventory>();

    player_world_interactions->LinkInventory(inventory);    
    player_world_interactions->LinkHealthBar(health_bar);
    player_world_interactions->LinkWorld(world);
    player_controller->LinkHealthBar(health_bar);
    player_controller->LinkWorld(world);
    player_controller->Respawn();

    // loading our player save file
    Serilizer::LoadPlayer(GetCurrentPlayer().filepath, this);

    scene->GetActiveCamera()->SetBackgroundTexture("background");

    // tell the world we want to focus around the player
    world->SetFocus(player->GetTransform());

    // create command console
    CommandParser::LinkClient(this);
    console_visual = scene->AddUI<ConsoleVisual>();


    world_loaded = true;
}


void GameClient::SetCurrentPlayer(Serilizer::DataPair player){
    this->current_player = player;
}
void GameClient::SetCurrentWorld(Serilizer::DataPair world){
    this->current_world = world;
}

void GameClient::SetAllowTimeout(bool _allow_timeout){


    if(this->allow_timeout == _allow_timeout){
        return;
    }

    this->allow_timeout = _allow_timeout;

    IF_ONLINE

    if(allow_timeout){
        SendPacket<PacketHeader>(server, {PACKET_EnableTimeout, client_id});
    }
    else{
        SendPacket<PacketHeader>(server, {PACKET_DisableTimeout, client_id});
    }
}


void GameClient::SendPlayerControl(){

    if(!world_loaded){
        return;
    }

    player_pos = player->GetTransform()->position;

    IF_ONLINE

    //if(player->GetTransform()->position != previous_player_position){
        
        previous_player_position = player->GetTransform()->position;
        auto anim = player->GetComponent<AnimationRenderer>();
        SendPacket<p_PlayerControl>(server, {{PACKET_PlayerControl, client_id}, anim->GetFlip(), anim->GetStateIndex(), player->GetTransform()->position.x, player->GetTransform()->position.y });
    //}
}

void GameClient::SendSetBlock(short tile_index, int x, int y){
    
    IF_ONLINE

    SendPacket<p_SetBlock>(server, {{PACKET_SetBlock, client_id}, tile_index, x, y});
}

void GameClient::SendChatMessage(const std::string& message){
    
    IF_ONLINE

    // having problems with strings in packets, ignore for now
    return;

    //char msg[50];
    //SendPacket<p_ChatMessage>(server, {{PACKET_ChatMessage, client_id}});
}


void GameClient::Update(){

    IF_ONLINE


    // we are sending chunks to a specific client
    if(transfer_chunks){

        chunk_transfer_x++;

        if(chunk_transfer_x >= world->GetWorldProfile()->width){
            chunk_transfer_x = 0;
            chunk_transfer_y++;
        }
        if(chunk_transfer_y < world->GetWorldProfile()->height){
        
            std::cout << "sent chunk " << chunk_transfer_x << " " << chunk_transfer_y << "\n";

            SendPacket<p_SetChunk>(server,  {{PACKET_SetChunk, client_id}, current_transfer_client_id, chunk_transfer_x, chunk_transfer_y, 
                world->GetChunks()->at(chunk_transfer_x).at(chunk_transfer_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->GetGrid(),
                world->GetChunks()->at(chunk_transfer_x).at(chunk_transfer_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->GetGrid(),
                world->GetChunks()->at(chunk_transfer_x).at(chunk_transfer_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->GetGrid(),
            });
        }
        else{
            transfer_chunks = false;
        }
    }
    else{
        // pick new client for transfer
        if(!players_to_transfer_chunks_to.empty()){

            current_transfer_client_id = players_to_transfer_chunks_to.front();
            players_to_transfer_chunks_to.pop();

            transfer_chunks = true;
            chunk_transfer_x = 0;
            chunk_transfer_y = 0;
        }
    }


    // if the world is not active we are yet not recieve all chunks, ask for the ones we missed
    // reuses the chunk_transfer_x and chunk_transfer_y variables, knowing the host will never have an inactive world
    if(!world_loaded){

        return;
        chunk_retry_delay_tracked++;

        if(chunk_retry_delay_tracked >= chunk_retry_delay){

            bool search_for_missing_chunk = true;

            while(search_for_missing_chunk) {

                if(chunk_transfer_x >= world->GetWorldProfile()->width){
                    chunk_transfer_x = 0;
                    chunk_transfer_y++;
                }
                if(chunk_transfer_y < world->GetWorldProfile()->height){
                
                    if(!chunks_success_grid[chunk_transfer_x][chunk_transfer_y]){

                        std::cout << "[CLIENT]: requesting chunk " << chunk_transfer_x << "," << chunk_transfer_y << "\n";

                        SendPacket<p_RequestSpecificChunk>(server, {{PACKET_RequestSpecificChunk, client_id}, chunk_transfer_x, chunk_transfer_y});
                        search_for_missing_chunk = false;
                    }
                }
                else{
                    search_for_missing_chunk = false;
                    chunk_retry_delay_tracked = 0;
                }
            }
        }

    }




    if(allow_timeout){

        // if the server hasn't responded in a while, timeout / drop connection
        if(time_since_last_packet > timeout_limit){
            std::cout << "[CLIENT]: timed out, disconnecting\n";

            Disconnect();
        }

        time_since_last_packet++;
    }
}

void GameClient::CatchPeerEvent(ENetEvent event){

    IF_ONLINE

    time_since_last_packet = 0;

    switch(event.type) 
    {   

        case ENET_EVENT_TYPE_RECEIVE: {

            InterpretPacket(event);

            enet_packet_destroy(event.packet);

            break;        
        }
    }
}



void GameClient::InterpretPacket(ENetEvent& event){
    
    IF_ONLINE

    // copy to the header portion of the message into PacketHeader object
    PacketHeader header;
    memcpy(&header, event.packet->data, sizeof(PacketHeader));

    // ignore messages from self
    if(header.client_id == client_id){
        return;
    }

    PACKET packet_type = (PACKET)header.type;

    switch(packet_type){

        case PACKET_ServerHeartbeat: {
            SendPacket<PacketHeader>(server, {PACKET_ServerHeartbeat, client_id});
            break;
        }

        case PACKET_SetClientId: {
            client_id = header.client_id;
            break;
        }
        case PACKET_CreatePlayer: {
            connected_clients[header.client_id] = scene->AddObject<Player>();
            break;
        }

        case PACKET_DeletePlayer: {
            scene->DeleteObject(connected_clients[header.client_id]);
            connected_clients.erase(header.client_id);
            break;
        }

        case PACKET_RequestWorldHeader: {
            std::cout << "[CLIENT]: requesting world header, PACKET_RequestWorldHeader\n";
            SendPacket<p_WorldHeader>(server, {{PACKET_WorldHeader, client_id}, header.client_id, world->GetWorldProfile()->width, world->GetWorldProfile()->height} );
            break;
        }
        

        case PACKET_WorldHeader : {
            
            std::cout << "[CLIENT]: recieved world header, PACKET_WorldHeader\n";
            
            p_WorldHeader body;
            memcpy(&body, event.packet->data, sizeof(p_WorldHeader));
            
            std::cout << "[CLIENT]: world size " << body.width << ", " << body.height << "\n";

            world->Create(false, body.width, body.height);

            chunks_success_grid.resize(body.width, {});
            for(int x = 0; x < body.width; x++){
                chunks_success_grid[x].resize(body.height, false);
            }

            chunk_retry_delay_tracked = 0;
            chunk_transfer_x = 0;
            chunk_transfer_y = 0;

            // we are now ready to recieve chunks
            SendPacket<PacketHeader>(server, {PACKET_RequestChunks, client_id});

            std::cout << "send request\n";
            break;
        }

        case PACKET_RequestSpecificChunk: {

            std::cout << "sending back requested chunk\n";

            p_RequestSpecificChunk body;
            memcpy(&body, event.packet->data, sizeof(p_RequestSpecificChunk));
            

            SendPacket<p_SetChunk>(server,  {{PACKET_SetChunk, client_id}, header.client_id, body.chunk_coordinate_x, body.chunk_coordinate_y, 
                world->GetChunks()->at(body.chunk_coordinate_x).at(body.chunk_coordinate_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->GetGrid(),
                world->GetChunks()->at(body.chunk_coordinate_x).at(body.chunk_coordinate_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->GetGrid(),
                world->GetChunks()->at(body.chunk_coordinate_x).at(body.chunk_coordinate_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->GetGrid(),
            });

            std::cout << "chunk sent\n";

            break;
        }

        case PACKET_SetChunk : {


            p_SetChunk body;
            memcpy(&body, event.packet->data, sizeof(p_SetChunk));

            std::cout << "setting chunk " << body.chunk_coordinate_x << " " << body.chunk_coordinate_y << "\n";


            world->GetChunks()->at(body.chunk_coordinate_x).at(body.chunk_coordinate_y)->GetTilemap(SetLocation::MAIN)->GetPrimitive()->CopyGrid(body.main_grid);
            world->GetChunks()->at(body.chunk_coordinate_x).at(body.chunk_coordinate_y)->GetTilemap(SetLocation::FOREGROUND)->GetPrimitive()->CopyGrid(body.foreground_grid);
            world->GetChunks()->at(body.chunk_coordinate_x).at(body.chunk_coordinate_y)->GetTilemap(SetLocation::BACKGROUND)->GetPrimitive()->CopyGrid(body.background_grid);



            if(world->ChunkInBounds(body.chunk_coordinate_x, body.chunk_coordinate_y)){
                
                // we have not recieved this chunk before
                if(!chunks_success_grid[body.chunk_coordinate_x][body.chunk_coordinate_y]){
                    chunks_recieved++;
                    chunks_success_grid[body.chunk_coordinate_x][body.chunk_coordinate_y] = true;
                }
            }


            std::cout << "recieved: " << chunks_recieved << "\n";

            // tell the server the world is loaded
            if(chunks_recieved == world->GetWorldProfile()->width * world->GetWorldProfile()->height){
                SendPacket<PacketHeader>(server, {PACKET_WorldLoadedSuccessfully, client_id});
                
                std::cout << "successfully recieved all chunks\n";

                world->SetActive(true);

                // we can now create game objects, with the world successfully loaded
                this->CreateObjects();
                world->CalculateMinimap();
                
                chunks_success_grid.clear();
            }

            break;
        }

        case PACKET_RequestChunks: {
            
            players_to_transfer_chunks_to.emplace(header.client_id);       
            break;
        }

        case PACKET_PlayerControl: {


            p_PlayerControl body;
            memcpy(&body, event.packet->data, sizeof(p_PlayerControl));

            // apply appropriate animation data
            auto anim = connected_clients[header.client_id]->GetComponent<AnimationRenderer>();
            anim->SetFlip(body.flip_sprite);
            anim->SetStateByIndex(0);
            
            // set position
            connected_clients[header.client_id]->GetTransform()->position = sf::Vector2f(body.pos_x, body.pos_y);
            connected_clients[header.client_id]->GetComponent<PhysicsBody>()->velocity = sf::Vector2f(body.velocity_x, body.velocity_y);

            break;
        }

        case PACKET_SetBlock : {

            p_SetBlock body;
            memcpy(&body, event.packet->data, sizeof(p_SetBlock));

            world->SetTile(body.tile_index, body.pos_x, body.pos_y, SetLocation::MAIN, SetMode::OVERRIDE, false);
            
            break;
        }


        case PACKET_ChatMessage : {
            


            break;
        }
    }
}

void GameClient::OnDisconnect(){

    IF_ONLINE
     
    //scene->DeleteObject(world);
}

void GameClient::SetPlayMode(PlayMode play_mode){
    this->play_mode = play_mode;
}