#include "../Amber/Framework.h"
#include "TestScene.h"

class Game : public Core{

    public:
        Game() : Core(1200, 800, 600, 400, "My Game"){}

        void Start() override{

            AssetManager::CreateTexture("cursor_breaking", "breaking.png");
            AssetManager::CreateTexture("demoTexture", "tiles.png");
            AssetManager::CreateTexture("player", "player.png");

            AssetManager::CreateSound("hit", "hit.wav");
            AssetManager::CreateSound("break", "break.wav");

            AssetManager::CreateScene<TestScene>("TestScene");

            LoadScene("TestScene");
        }

        void CatchEvent(sf::Event event) override{
            
            // restart scene with 'E'
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::E){
                    LoadScene("TestScene");
                }
            }
        }
};   