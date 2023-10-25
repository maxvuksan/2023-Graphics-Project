#include "../Amber/Framework.h"
#include "TestScene.h"

class Game : public Core{

    public:
        Game() : Core(1200, 800, 300, 200, "My Game"){}

        void Start() override{

            AssetManager::CreateTexture("cursor_breaking", "breaking.png");
            AssetManager::CreateTexture("demoTexture", "tiles.png");

            AssetManager::CreateSound("hit", "hit.wav");
            AssetManager::CreateSound("break", "break.wav");

            AssetManager::CreateScene<TestScene>("TestScene");

            LoadScene("TestScene");
        }
};  