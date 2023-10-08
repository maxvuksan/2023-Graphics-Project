#include "Game.h"
#include "TestScene.h"

void Game::Start(){
    AssetManager::CreateTexture("light_shaft", "light_shaft.png");
    AssetManager::CreateTexture("demoTexture", "demo.png");
    
    AssetManager::CreateScene<TestScene>("TestScene");

    LoadScene("TestScene");
}

void Game::Update(){
    
}
