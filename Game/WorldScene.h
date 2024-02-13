#pragma once

#include "../Amber/Framework.h"
#include "Player/Player.h"
#include "Player/PlayerController.h"
#include "World/World.h"
#include <math.h>
#include "Networking/GameClient.h"
#include "Serilizer.h"

/*
    The gameplay scene (where a world will be loaded)
*/
class WorldScene : public Scene {

    public: 

        void Start() override;

        void Update() override;

        void CatchEvent(sf::Event event) override;

    private:


        UIRectArray* pause_rect_array;
        UIButton* return_button;
        UIButton* quit_button;

        World* world;
        GameClient* client;
};