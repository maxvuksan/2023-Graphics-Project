#include "../Amber/Framework.h"
#include "Networking/GameServer.h"
#include "Networking/GameClient.h"
#include "WorldScene.h"

class Game : public Core{

    public:
        Game(bool is_server = false) : Core(1200, 800, 600, 400, "My Game")
        {
            if(is_server){
               server.Run(6868); //    SendPacket<BodyTest>(server, {{7}, 123, 456, 789});
            }
        }

        ~Game() override{ 
            server.Close();
        }

        void Start() override{

            AssetManager::CreateTexture("cursor_breaking", "breaking.png");
            AssetManager::CreateTexture("tiles", "tiles.png");
            AssetManager::CreateTexture("player", "player.png");

            AssetManager::CreateTexture("pickups", "pickups.png");

            AssetManager::CreateTexture("fly", "fly.png");
            AssetManager::CreateAnimationSet("fly", { Animation("fly", 4, 0, 2, 25)}, {"default"});

            AssetManager::CreateSound("hit", "hit.wav");
            AssetManager::CreateSound("break", "break.wav");

            AssetManager::CreateScene<WorldScene>("WorldScene");

            LoadScene("WorldScene");

        }

        GameServer server;
};   