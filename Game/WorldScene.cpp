#include "WorldScene.h"
#include "Game.h"
#include "Serilizer.h"

void WorldScene::Start(){

    Camera* camera = AddObject<Object>()->AddComponent<Camera>();
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
//            world->CalculateMinimap();

            break;
        }

        case PlayMode::HOSTING : {

            Serilizer::LoadWorldIntoMemory(client->GetCurrentWorld().filepath, world);
            
            client->Connect("127.0.0.1", 6868);
            client->SetAllowTimeout(false);

            client->CreateObjects();
  //          world->CalculateMinimap();


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
    world->CalculateMinimap();
}

void WorldScene::Update(){

   client->SendPlayerControl();
}


void WorldScene::CatchEvent(sf::Event event){

    if(event.type == sf::Event::KeyPressed){
        if(event.key.scancode == sf::Keyboard::Scancode::Escape){
            Serilizer::SaveWorld(client->GetCurrentWorld(), world);
        }
    }
}