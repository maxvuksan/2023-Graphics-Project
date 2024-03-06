#include "WorldScene.h"
#include "Game.h"
#include "Serilizer.h"

void WorldScene::Start(){

    camera = AddObject<Object>()->AddComponent<Camera>();
    camera->background_colour = sf::Color(42, 40, 48);
    SetActiveCamera(camera);

    client = Game::GetGameClientFromScene(this);
    client->LinkScene(this);

    world = AddObject<World>();
    world->LinkClient(client);

    client->LinkWorld(world);

    // singleplayer
    switch(client->GetPlayMode()){

        case PlayMode::OFFLINE : {

            client->SetAllowTimeout(false);
            Serilizer::LoadWorldIntoMemory(client->GetCurrentWorld().filepath, world);

            client->CreateObjects();
            world->CalculateMinimap();

            break;
        }

        case PlayMode::HOSTING : {

            Serilizer::LoadWorldIntoMemory(client->GetCurrentWorld().filepath, world);
            client->Connect("127.0.0.1", 6868);
            client->SetAllowTimeout(false);

            client->CreateObjects();
            world->CalculateMinimap();

            break;
        }

        case PlayMode::JOINING: {

            // stop the world from updating while it has not fully loaded
            world->SetActive(false);

            client->Connect("127.0.0.1", 6868);
            client->SetAllowTimeout(false);
            break;
        }
    }

    client->SetAllowTimeout(true);
    
    // pause menu setup

    return_button = AddUI<Object>()->AddComponent<UIButton>();
    return_button->SetString("Return");
    return_button->SetOnClickCallback([this](){
        
        pause_rect_array->GetThisObject()->SetActive(false);

    });

    quit_button = AddUI<Object>()->AddComponent<UIButton>();
    quit_button->SetString("Save & Quit");
    quit_button->SetOnClickCallback([this](){
        
        Serilizer::SaveWorld(client->GetCurrentWorld(), world);
        Core::LoadScene("MenuScene");
    });

    pause_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    pause_rect_array->SetElementSize(150, 50);
    pause_rect_array->SetGap(UIRect::padding);
    pause_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    
    pause_rect_array->AddUIRect(quit_button);
    pause_rect_array->AddUIRect(return_button);

    pause_rect_array->SetActive(false);

    player_list_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    player_list_rect_array->SetElementSize(150, 25);
    player_list_rect_array->SetGap(UIRect::padding);
    player_list_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    player_list_rect_array->SetActive(false);
}


void WorldScene::Update(){

    // the player does not exist yet
    if(client->GetPlayer() == nullptr || world->GetFocus() == nullptr){
        return;
    }

   client->SendPlayerControl();

    /*
    if(camera->GetThisObject()->GetTransform()->position != client->GetPlayer()->GetTransform()->position){
        
        camera->GetThisObject()->GetTransform()->position = Calc::Lerp(camera->GetThisObject()->GetTransform()->position, client->GetPlayer()->GetTransform()->position, 0.1);
    }
    */
   // camera lerping between chunks ---------------------------------------------------------------------------------------------------


    sf::Vector2i player_coord = world->WorldToCoord(world->GetFocus()->position.x, world->GetFocus()->position.y);
    sf::Vector2i chunk_coordinate = world->ChunkFromCoord(player_coord.x, player_coord.y);


    int camera_move_threshhold = Settings::CAMERA_CHUNK_BLENDING;

    
    float player_x = world->GetFocus()->position.x / world->GetWorldProfile()->tilemap_profile.tile_width;
    float player_y = world->GetFocus()->position.y / world->GetWorldProfile()->tilemap_profile.tile_height;

    player_x = fmod(player_x, world->GetWorldProfile()->tilemap_profile.width);
    player_y = fmod(player_y, world->GetWorldProfile()->tilemap_profile.height);

    sf::Vector2f camera_shift_factor;

    if(player_x < camera_move_threshhold){

        float t = 1 - player_x / (float)camera_move_threshhold;
        camera_shift_factor.x = Calc::Lerp(world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->tilemap_profile.tile_width  / 2.0f,
                                            0,
                                            t * t * t);
    }
    else if(player_x > world->GetWorldProfile()->tilemap_profile.width - camera_move_threshhold){
       
        float t = (player_x - world->GetWorldProfile()->tilemap_profile.width + camera_move_threshhold) / (float)camera_move_threshhold;


        camera_shift_factor.x = Calc::Lerp(world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->tilemap_profile.tile_width / 2.0f, 
                                            world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->tilemap_profile.tile_width, 
                                            t * t * t);
    }
    else{
        camera_shift_factor.x = world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->tilemap_profile.tile_width / 2.0f;
    }

    /*
    if(player_y < camera_move_threshhold){
        float t = player_y / (float)camera_move_threshhold;

        camera_shift_factor.y = Calc::Lerp(world->GetWorldProfile()->tilemap_profile.height * world->GetWorldProfile()->tilemap_profile.tile_height / 2.0f,
                                            0, 
                                            Calc::EaseOutCubic(t));
    }
    else if(player_y > world->GetWorldProfile()->tilemap_profile.height - camera_move_threshhold){

        float t = (player_y - world->GetWorldProfile()->tilemap_profile.height + camera_move_threshhold) / (float)camera_move_threshhold;


        camera_shift_factor.y = Calc::Lerp(world->GetWorldProfile()->tilemap_profile.height * world->GetWorldProfile()->tilemap_profile.tile_height / 2.0f, 
                                            world->GetWorldProfile()->tilemap_profile.height * world->GetWorldProfile()->tilemap_profile.tile_height, 
                                            Calc::EaseOutCubic(t));
    }
    else{
        camera_shift_factor.y = world->GetWorldProfile()->tilemap_profile.height * world->GetWorldProfile()->tilemap_profile.tile_height / 2.0f;
    }
    */
    camera->GetThisObject()->GetTransform()->position.x = 
        chunk_coordinate.x * world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->tilemap_profile.tile_width//, chunk_coordinate.y * world->GetWorldProfile()->tilemap_profile.height * world->GetWorldProfile()->tilemap_profile.tile_height ) 
        + camera_shift_factor.x;

    camera->GetThisObject()->GetTransform()->position.y = Calc::Lerp(camera->GetThisObject()->GetTransform()->position.y, client->GetPlayer()->GetTransform()->position.y, 0.1);
}


void WorldScene::CatchEvent(sf::Event event){

    if(event.type == sf::Event::KeyPressed){
        if(event.key.scancode == sf::Keyboard::Scancode::S){ 

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            {
                Serilizer::SaveWorld(client->GetCurrentWorld(), world);
            }
        }

        // opening pause
        if(event.key.scancode == sf::Keyboard::Scancode::Escape){ 

            if(pause_rect_array->GetThisObject()->IsActive()){
                pause_rect_array->GetThisObject()->SetActive(false);
                Scene::GetActiveCamera()->ui_overlay_colour = sf::Color(0,0,0,0);
            }
            else{
                pause_rect_array->GetThisObject()->SetActive(true);
                Scene::GetActiveCamera()->ui_overlay_colour = sf::Color(0,0,0,150);
            }
        }

        // opening player list
        if(event.key.scancode == sf::Keyboard::Scancode::Tab){

            std::cout << "tab presed\n";

            // we are playing online
            if(client->GetPlayMode() != PlayMode::OFFLINE){


                UIButton* myself = AddUI<Object>()->AddComponent<UIButton>();
                myself->SetString("me");
                player_list_rect_array->AddUIRect(myself);

                std::cout << "num of clients: " << client->GetConnectedClients().size() << "\n";
                // add new rects
                for(auto client : client->GetConnectedClients()){
                    std::cout << client.first << "\n";

                    UIButton* new_player = AddUI<Object>()->AddComponent<UIButton>();
                    new_player->SetString("" + client.first);

                    player_list_rect_array->AddUIRect(new_player);
                }

                player_list_rect_array->SetActive(true);
            }

        }
    
    }
    if(event.type == sf::Event::KeyReleased){
        
        // closing player list
        if(event.key.scancode == sf::Keyboard::Scancode::Tab){

            for(auto rect : player_list_rect_array->GetUIRects()){
                DeleteObject(rect->GetThisObject());
            }
            player_list_rect_array->ClearUIRects();

            player_list_rect_array->SetActive(false);
        }
    }

}
