#include "../Amber/Framework.h"
#include "Networking/GameServer.h"
#include "Networking/GameClient.h"
#include "TestScene.h"

class Game : public Core{

    public:
        Game(bool is_server = false) : Core(1200, 800, 600, 400, "My Game")
        {
            if(is_server){
                server.Run(6868); //    SendPacket<BodyTest>(server, {{7}, 123, 456, 789});
            }
            client.Connect("127.0.0.1", 6868);
            client.SendStuff();
        }

        ~Game() override{ 
            server.Close();
        }

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

        GameServer server;
        GameClient client;
};   