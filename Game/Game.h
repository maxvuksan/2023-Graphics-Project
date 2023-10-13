#include "../Amber/Framework.h"
#include "TestScene.h"

class Game : public Core{

    public:
        Game() : Core(900, 900, 300, 300, "My Game"){}

        void Start() override{

            AssetManager::CreateTexture("cursor_breaking", "breaking.png");
            AssetManager::CreateTexture("demoTexture", "tiles.png");

            AssetManager::CreateSound("hit", "hit.wav");
            AssetManager::CreateSound("break", "break.wav");

            AssetManager::CreateScene<TestScene>("TestScene");


            AssetManager::CreateAnimationSet(
                "cursor_breaking", 
                {Animation("cursor_breaking", 8, 0, 4, 100)}, 
                
                {"breaking"}
            );


            LoadScene("TestScene");
        }
}; 