#pragma once

#include "../Amber/Framework.h"
#include "Player/Player.h"
#include "Player/PlayerController.h"
#include "World/World.h"
#include <math.h>
#include "Networking/GameClient.h"


/*
    The gameplay scene (where a world will be loaded)
*/
class WorldScene : public Scene {

    public: 

        void Start() override;

        void Update() override;

    private:

        World* world;
        GameClient* client;
};