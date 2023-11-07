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

        void CatchEvent(sf::Event event) override{
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.scancode == sf::Keyboard::Scan::Q){
                    Core::DEBUG_MODE = !Core::DEBUG_MODE;
                }
            }
        }

    private:
        GameClient client;
};