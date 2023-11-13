#include "../Amber/Framework.h"
#include "Player.h"
#include "PlayerController.h"
#include "World.h"
#include <math.h>
#include "Networking/GameClient.h"

class TestScene : public Scene {

    public: 

        void Start() override{

           client.LinkScene(this);
           client.CreateObjects();
           client.Connect("127.0.0.1", 6868);

        };

        void Update() override{

            client.SendPlayerControl();
        }

    private:
        GameClient client;
};