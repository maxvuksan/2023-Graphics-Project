#include "../Amber/Framework.h"
#include "Player/Player.h"
#include "Player/PlayerController.h"
#include "World/World.h"
#include <math.h>
#include "Networking/GameClient.h"

#include "GameUI/SlotSet.h"

/*
    The gameplay scene (where a world will be loaded)
*/
class WorldScene : public Scene {

    public: 

        void Start() override{

            Slot::Construct();
            SlotSet::Construct();


            client.LinkScene(this);

            world = AddObject<World>();
            world->LinkWorldScene(this);
            world->LinkClient(&client);
            world->Create();

            client.LinkWorld(world);
        
            client.CreateObjects();
            client.Connect("127.0.0.1", 6868);


        };

        void Update() override{

            client.SendPlayerControl();
        }

    private:

        World* world;
        GameClient client;
};