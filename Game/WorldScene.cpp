#include "WorldScene.h"
#include "Game.h"
#include "Serilizer.h"
#include "World/Lighting/LightingManager.h"

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
            client->Connect("127.0.0.1", 27015);
            client->SetAllowTimeout(false);

            client->CreateObjects();
            world->CalculateMinimap();

            break;
        }

        case PlayMode::JOINING: {

            // stop the world from updating while it has not fully loaded
            world->SetActive(false);

            std::cout << client->GetTargetIPAddress() << " " << client->GetTargetPort() << "\n";

            client->Connect(client->GetTargetIPAddress().c_str(), client->GetTargetPort());
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


    light_render_texture.create(Core::GetDisplayWidth() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2, Core::GetDisplayHeight() + RenderManager::GetCameraSmoothingEdgeBuffer() * 2);
}

void WorldScene::DrawAtFullResolution(sf::RenderTarget& surface){


    // drawing players ________________________________________________________________________

    for(auto map_item : client->GetConnectedClients()){

        Player* player = map_item.second;

        sf::Sprite player_render_sprite(player->GetPlayerRenderTexture().getTexture());

        // use no floor to get smooth value
        sf::Vector2f position = Camera::WorldToScreenPositionNoFloor(player->GetTransform()->position) - sf::Vector2f(50, 51);
        sf::Vector2f final_pos = sf::Vector2f(position.x * Core::GetDisplayToWindowMultiplier().x, position.y * Core::GetDisplayToWindowMultiplier().y);
        player_render_sprite.setPosition(final_pos);

        sf::Vector2f cam_pos = Scene::GetActiveCamera()->GetBoundedPosition(); //sf::Vector2f(abs(Scene::GetActiveCamera()->GetBoundedPosition().x - final_pos.x), abs(Scene::GetActiveCamera()->GetBoundedPosition().y - final_pos.y));
        cam_pos.x *= Core::GetDisplayToWindowMultiplier().x;
        cam_pos.y *= Core::GetDisplayToWindowMultiplier().y;
        
        player_render_sprite.setPosition( player_render_sprite.getPosition() + sf::Vector2f(
        floor(cam_pos.x) - cam_pos.x, 
        floor(cam_pos.y) - cam_pos.y ) + sf::Vector2f(0, 1));
    
        player_render_sprite.setScale(Core::GetDisplayToWindowMultiplier());

        surface.draw(player_render_sprite);
    }

    // finally drawing lighting ______________________________________________________________
    
    if(LightingManager::show_lighting){
        light_render_texture.clear(sf::Color::Transparent);

        LightingManager::_Draw(light_render_texture);
        light_render_texture.display();

        sf::Sprite light_sprite(light_render_texture.getTexture());
        light_sprite.setScale(Core::GetDisplayToWindowMultiplier());
        light_sprite.setPosition(RenderManager::GetCameraSmoothingVector());

        surface.draw(light_sprite, sf::BlendMultiply);
    }

}


void WorldScene::Update(){

    // the player does not exist yet
    if(client->GetPlayer() == nullptr || world->GetFocus() == nullptr){
        return;
    }

    client->SendPlayerControl();

    camera->GetThisObject()->GetTransform()->position.y = Calc::Lerp(camera->GetThisObject()->GetTransform()->position.y, client->GetPlayer()->GetTransform()->position.y, 0.03);
    camera->GetThisObject()->GetTransform()->position.x = Calc::Lerp(camera->GetThisObject()->GetTransform()->position.x, client->GetPlayer()->GetTransform()->position.x, 0.03);
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
