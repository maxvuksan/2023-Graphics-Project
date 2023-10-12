#include "../Amber/Framework.h"
#include "TestScene.h"

class Game : public Core{

    public:
        Game() : Core(900, 900, 300, 300, "My Game"){}

        void Start() override{
            AssetManager::CreateTexture("demoTexture", "tiles.png");
            
            AssetManager::CreateScene<TestScene>("TestScene");

            LoadScene("TestScene");
        }
}; 