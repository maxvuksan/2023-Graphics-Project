#include "Game.h"
#include "TestScene.h"

void Game::Start(){
    GetAssetManager()->CreateTexture("demoTexture", "demo.png");

    GetAssetManager()->CreateScene<TestScene>("TestScene");
    LoadScene("TestScene");
    
    
}

void Game::Update(){
    
}
