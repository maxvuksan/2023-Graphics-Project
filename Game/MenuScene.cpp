#include "MenuScene.h"
#include "Game.h"


void MenuScene::Start(){


    client = Game::GetGameClientFromScene(this);

    Camera* camera = AddObject<Object>()->AddComponent<Camera>();
    camera->background_colour = sf::Color(42, 20, 48);
    SetActiveCamera(camera);


    // Main --------------------------------------------------------------------------

    play_button = AddUI<Object>()->AddComponent<UIButton>();
    play_button->SetString("Play");

    play_button->SetOnClickCallback([this](){

        this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);
    });

    settings_button = AddUI<Object>()->AddComponent<UIButton>();
    settings_button->SetString("Settings");
    settings_button->SetOnClickCallback([](){
        std::cout << "settings pressed\n";
    });

    main_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    main_rect_array->AddUIRect(play_button);
    main_rect_array->AddUIRect(settings_button);
    main_rect_array->SetElementSize(100, 50);
    main_rect_array->SetGap(UIRect::padding);
    main_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);

    // select play mode -----------------------------------------------------------

    offline_button = AddUI<Object>()->AddComponent<UIButton>();
    offline_button->SetString("Offline");

    offline_button->SetOnClickCallback([this](){
        play_mode = PlayMode::OFFLINE;
        this->SwitchMenuState(MenuStates::SELECT_PLAYER);
        std::cout << "offline pressed\n";
    });

    join_button = AddUI<Object>()->AddComponent<UIButton>();
    join_button->SetString("Join");

    join_button->SetOnClickCallback([this](){
        play_mode = PlayMode::JOINING;
        this->SwitchMenuState(MenuStates::SELECT_PLAYER);
        std::cout << "join pressed\n";
    });

    host_button = AddUI<Object>()->AddComponent<UIButton>();
    host_button->SetString("Host");

    host_button->SetOnClickCallback([this](){
        std::cout << "host pressed\n";
        play_mode = PlayMode::HOSTING;
        this->SwitchMenuState(MenuStates::SELECT_PLAYER);
    });

    play_mode_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    play_mode_rect_array->AddUIRect(offline_button);
    play_mode_rect_array->AddUIRect(join_button);
    play_mode_rect_array->AddUIRect(host_button);
    play_mode_rect_array->SetElementSize(100, 50);
    play_mode_rect_array->SetGap(UIRect::padding);
    play_mode_rect_array->SetAlign(ScreenLocationX::LEFT, ScreenLocationY::CENTER);


    // Select Player ----------------------------------------------------------------

    new_player_button = AddUI<Object>()->AddComponent<UIButton>();
    new_player_button->SetString("+ New Player");
    new_player_button->SetOnClickCallback([this](){
        
        this->SwitchMenuState(MenuStates::CREATE_PLAYER);
    });       

    select_player_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    select_player_rect_array->SetElementSize(150, 25);
    select_player_rect_array->SetGap(UIRect::padding);
    select_player_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);

    // Create Player ----------------------------------------------------------------

    create_player_input = AddUI<Object>()->AddComponent<UIInputField>();
    create_player_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    create_player_input->SetPlaceholder("Enter Name...");
    
    create_player_button = AddUI<Object>()->AddComponent<UIButton>();
    create_player_button->SetString("Create");
    create_player_button->SetOnClickCallback([this](){
        
        // create new player save file
        if(create_player_input->GetInput() != ""){
            Serilizer::CreateNewPlayer(create_player_input->GetInput());
            this->SwitchMenuState(MenuStates::SELECT_PLAYER);
            create_player_input->SetInput("");
        }
    });

    create_player_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    create_player_rect_array->SetElementSize(150, 50);
    create_player_rect_array->SetGap(UIRect::padding);
    create_player_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    create_player_rect_array->AddUIRect(create_player_input);
    create_player_rect_array->AddUIRect(create_player_button);

    // Select World ------------------------------------------------------------------

    new_world_button = AddUI<Object>()->AddComponent<UIButton>();
    new_world_button->SetString("+ New World");
    new_world_button->SetOnClickCallback([this](){
        
        this->SwitchMenuState(MenuStates::CREATE_WORLD);
    });       

    select_world_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    select_world_rect_array->SetElementSize(150, 25);
    select_world_rect_array->SetGap(UIRect::padding);
    select_world_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);

    // Create World ------------------------------------------------------------------

    create_world_input = AddUI<Object>()->AddComponent<UIInputField>();
    create_world_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    create_world_input->SetPlaceholder("Enter World Name...");
    
    create_world_button = AddUI<Object>()->AddComponent<UIButton>();
    create_world_button->SetString("Create");
    create_world_button->SetOnClickCallback([this](){
        
        if(create_world_input->GetInput() != ""){


            World* world = AddObject<World>();
            world->Create(true);
            world->Generate();

            Serilizer::CreateNewWorld(create_world_input->GetInput(), world);

            this->SwitchMenuState(MenuStates::SELECT_WORLD);
            create_world_input->SetInput("");

            DeleteObject(world);
        }

    });
    create_world_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    create_world_rect_array->SetElementSize(150, 50);
    create_world_rect_array->SetGap(UIRect::padding);
    create_world_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    create_world_rect_array->AddUIRect(create_world_input);
    create_world_rect_array->AddUIRect(create_world_button);



    new_world_button->SetActive(false);
    new_player_button->SetActive(false);
    create_world_button->SetActive(false);
    create_player_button->SetActive(false);


    this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);


}

void MenuScene::EnterWorld(){
    this->client->SetPlayMode(this->play_mode);
    Game::LoadScene("WorldScene");
}

void MenuScene::ClearFoundPlayersList(){
    select_player_rect_array->ClearUIRects();
    found_players.clear();

    // deletes buttons
    for(int i = 0; i < found_players_buttons.size(); i++){
        DeleteObject(found_players_buttons[i]->GetThisObject());
    }

    found_players_buttons.clear();
}

void MenuScene::ClearFoundWorldsList(){
    select_world_rect_array->ClearUIRects();
    found_worlds.clear();

    // deletes buttons
    for(int i = 0; i < found_worlds_buttons.size(); i++){
        DeleteObject(found_worlds_buttons[i]->GetThisObject());
    }

    found_worlds_buttons.clear();
}

void MenuScene::SwitchMenuState(MenuStates new_state){

    switch(new_state){

        case MAIN : {
            main_rect_array->SetActive(true);
            
            select_player_rect_array->SetActive(false);
            create_player_rect_array->SetActive(false);
            select_world_rect_array->SetActive(false);
            create_world_rect_array->SetActive(false);
            play_mode_rect_array->SetActive(false);
            break;
        }
        case SELECT_PLAY_MODE: {
            main_rect_array->SetActive(false);
            select_player_rect_array->SetActive(false);
            create_player_rect_array->SetActive(false);
            select_world_rect_array->SetActive(false);
            create_world_rect_array->SetActive(false);
            play_mode_rect_array->SetActive(true);   
            break;
        }
        case SELECT_PLAYER : {

            ClearFoundPlayersList();

            select_player_rect_array->AddUIRect(new_player_button);

            // add a button for each player

            found_players = Serilizer::LoadPlayerList();
            for(int i = 0; i < found_players.size(); i++){

                UIButton* button = AddObject<Object>()->AddComponent<UIButton>();
                button->SetString(found_players[i].name);

                // choose our player
                button->SetOnClickCallback([this, i](){

                    client->SetCurrentPlayer(this->found_players[i]);

                    if(play_mode == JOINING){
                        this->EnterWorld();
                    }
                    else{
                        this->SwitchMenuState(MenuStates::SELECT_WORLD);
                    }
                });

                select_player_rect_array->AddUIRect(button);
                found_players_buttons.push_back(button);
            }

            select_player_rect_array->SetActive(true);
            main_rect_array->SetActive(false);
            create_player_rect_array->SetActive(false);
            select_world_rect_array->SetActive(false);
            create_world_rect_array->SetActive(false);
            play_mode_rect_array->SetActive(false);
            break;
        }
        case CREATE_PLAYER : {
            select_player_rect_array->SetActive(false);
            main_rect_array->SetActive(false);
            select_world_rect_array->SetActive(false);
            create_world_rect_array->SetActive(false);

            create_player_rect_array->SetActive(true);
            play_mode_rect_array->SetActive(false);
            break;
        }
        case SELECT_WORLD : {

            ClearFoundWorldsList();

            select_world_rect_array->AddUIRect(new_world_button);

            // add a button for each player

            found_worlds = Serilizer::LoadWorldList();
            for(int i = 0; i < found_worlds.size(); i++){

                UIButton* button = AddObject<Object>()->AddComponent<UIButton>();
                button->SetString(found_worlds[i].name);

                // join the world
                button->SetOnClickCallback([this, i](){
                    
                    client->SetCurrentWorld(this->found_worlds[i]);

                    if(this->play_mode == PlayMode::HOSTING){
                        auto server = Game::GetGameServerFromScene(this);
                        if(server == nullptr){
                            std::cout << "ERROR : server has returned nullptr, WorldScene::Start()\n";
                        }

                        server->Run(6868);
                    }

                    this->EnterWorld();
                    
                });

                select_world_rect_array->AddUIRect(button);
                found_worlds_buttons.push_back(button);
            }

            main_rect_array->SetActive(false);
            select_player_rect_array->SetActive(false);
            create_player_rect_array->SetActive(false);
            create_world_rect_array->SetActive(false);

            play_mode_rect_array->SetActive(false);
            select_world_rect_array->SetActive(true);

            break;
        }
        case CREATE_WORLD : {
            main_rect_array->SetActive(false);
            select_player_rect_array->SetActive(false);
            create_player_rect_array->SetActive(false);
            create_world_rect_array->SetActive(true);
            select_world_rect_array->SetActive(false);
            play_mode_rect_array->SetActive(false);
            break;
        }
    }
    

    this->menu_state = new_state;
}