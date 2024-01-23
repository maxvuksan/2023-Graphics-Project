#include "WorldScene.h"
#include "Game.h"


void WorldScene::Start(){

    client = Game::GetGameClientFromScene(this);
    client->SetAllowTimeout(false);
    client->LinkScene(this);

    world = AddObject<World>();
    world->LinkWorldScene(this);
    world->LinkClient(client);


    client->SetAllowTimeout(true);

    client->LinkWorld(world);
    client->CreateObjects();

};

void WorldScene::Update(){

    client->SendPlayerControl();
}