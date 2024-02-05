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
    world->Create(false, 10,10);



    // singleplayer
    if(client->GetPlayMode() == PlayMode::OFFLINE){
        client->SetAllowTimeout(false);
        Serilizer::LoadWorldIntoMemory(client->GetCurrentWorld().filepath, world);
    }


    // multiplayer
    else if(client->GetPlayMode() == PlayMode::HOSTING || client->GetPlayMode() == PlayMode::JOINING){


        client->Connect("127.0.0.1", 6868);
        client->SetAllowTimeout(false);



        //Serilizer::LoadWorldIntoMemory(client->GetCurrentWorld().filepath, world);
        //Serilizer::LoadWorldIntoMemory(client->GetCurrentWorld().filepath, world);
    }



    client->SetAllowTimeout(true);

    client->LinkWorld(world);
    client->CreateObjects();

    Serilizer::LoadPlayer(client->GetCurrentPlayer().filepath, client);
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