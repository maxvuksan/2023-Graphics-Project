#include "MenuScene.h"
#include "Game.h"


void MenuScene::Start(){


    client = Game::GetGameClientFromScene(this);

    Camera* camera = AddObject<Object>()->AddComponent<Camera>();
    camera->background_colour = sf::Color(42, 20, 48);
    SetActiveCamera(camera);


    // Main --------------------------------------------------------------------------




    // Select Player ----------------------------------------------------------------

    new_player_button = AddUI<Object>()->AddComponent<UIButton>();
    new_player_button->SetString("+ New Player");
    new_player_button->SetOnClickCallback([this](){
        
        this->SwitchMenuState(MenuStates::CREATE_PLAYER);
    });       


    // Create Player ----------------------------------------------------------------

    create_player_input = AddUI<Object>()->AddComponent<UIInputField>();
    //create_player_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
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

    // Select World ------------------------------------------------------------------

    new_world_button = AddUI<Object>()->AddComponent<UIButton>();
    new_world_button->SetString("+ New World");
    new_world_button->SetOnClickCallback([this](){
        
        this->SwitchMenuState(MenuStates::CREATE_WORLD);
    });       

    // Create World ------------------------------------------------------------------

    create_world_input = AddUI<Object>()->AddComponent<UIInputField>();
    //create_world_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
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




    // Select World ------------------------------------------------------------------

    new_structure_button = AddUI<Object>()->AddComponent<UIButton>();
    new_structure_button->SetString("+ New Structure");
    new_structure_button->SetOnClickCallback([this](){
        
        this->SwitchMenuState(MenuStates::CREATE_STRUCTURE);
    });       


    // Create Structure ------------------------------------------------------------------

    create_structure_input = AddUI<Object>()->AddComponent<UIInputField>();
    //create_structure_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    create_structure_input->SetPlaceholder("Enter Structure Name...");
    
    create_structure_button = AddUI<Object>()->AddComponent<UIButton>();
    create_structure_button->SetString("Create");
    create_structure_button->SetOnClickCallback([this](){
        
        if(create_structure_input->GetInput() != ""){

            World* world = AddObject<World>();
            world->Create(true, 1, 1);

            Serilizer::CreateNewStructure(create_structure_input->GetInput(), world);

            this->SwitchMenuState(MenuStates::SELECT_STRUCTURE);
            create_structure_input->SetInput("");

            DeleteObject(world);
        }

    });

    // Create Structure ------------------------------------------------------------------

    join_ip_input = AddUI<Object>()->AddComponent<UIInputField>();
    //join_ip_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    join_ip_input->SetPlaceholder("Host IP Address...");

    join_port_input = AddUI<Object>()->AddComponent<UIInputField>();
    //join_ip_input->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    join_port_input->SetPlaceholder("Port...");
   

    join_ip_button = AddUI<Object>()->AddComponent<UIButton>();
    join_ip_button->SetString("Submit IP");
    join_ip_button->SetOnClickCallback([this](){
        
        if(join_ip_input->GetInput() != ""){

            client->SetTargetIPAddress(join_ip_input->GetInput());

            try {
                // Convert string to integer
                int port = std::stoi(join_port_input->GetInput());
                
                client->SetTargetPort(port);
                
                this->SwitchMenuState(MenuStates::SELECT_PLAYER);
                

            } 
            catch (const std::invalid_argument& e) {
                // Handle the case where the string does not represent a valid integer
                std::cout << "ERROR: invalid port number, MenuScene::Start()\n";
            } 
            catch (const std::out_of_range& e) {
                // Handle the case where the string represents an integer out of the range of int
                std::cout << "ERROR: port number out of range, MenuScene::Start()\n";
            }

            
            
        }

    });

    // select play mode -----------------------------------------------------------

    offline_button = AddUI<Object>()->AddComponent<UIButton>();
    offline_button->SetString("Offline");

    offline_button->SetOnClickCallback([this](){
        play_mode = PlayMode::OFFLINE;
        this->SwitchMenuState(MenuStates::SELECT_PLAYER);
    });

    join_button = AddUI<Object>()->AddComponent<UIButton>();
    join_button->SetString("Join");

    join_button->SetOnClickCallback([this](){
        play_mode = PlayMode::JOINING;
        this->SwitchMenuState(MenuStates::ENTER_JOIN_IP);
    });

    host_button = AddUI<Object>()->AddComponent<UIButton>();
    host_button->SetString("Host");

    host_button->SetOnClickCallback([this](){
        play_mode = PlayMode::HOSTING;
        this->SwitchMenuState(MenuStates::SELECT_PLAYER);
    });

    structure_button = AddUI<Object>()->AddComponent<UIButton>();
    structure_button->SetString("Dev");

    structure_button->SetOnClickCallback([this](){
        play_mode = PlayMode::OFFLINE;
        this->SwitchMenuState(MenuStates::SELECT_STRUCTURE);
    });

    rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
    rect_array->SetElementSize(150, 30);
    rect_array->SetGap(UIRect::padding);
    rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);


    new_world_button->SetActive(false);
    new_player_button->SetActive(false);
    create_world_button->SetActive(false);
    create_player_button->SetActive(false);
    create_player_input->SetActive(false);
    create_world_input->SetActive(false);
    new_structure_button->SetActive(false);
    create_structure_button->SetActive(false);
    create_structure_input->SetActive(false);
    
    join_ip_input->SetActive(false);
    join_ip_button->SetActive(false);
    join_port_input->SetActive(false);

    this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);
}

void MenuScene::CatchEvent(sf::Event event){


    if(event.type == sf::Event::KeyPressed){

        if(event.key.scancode == sf::Keyboard::Scancode::Escape){

            switch(menu_state){

                case MenuStates::SELECT_PLAY_MODE:
                    break;

                case MenuStates::ENTER_JOIN_IP: 
                    this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);
                    break;

                case MenuStates::SELECT_WORLD:
                    this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);
                    break;                 

                case MenuStates::SELECT_PLAYER:
                    this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);
                    break;      

                case MenuStates::SELECT_STRUCTURE:
                    this->SwitchMenuState(MenuStates::SELECT_PLAY_MODE);
                    break;     

                case MenuStates::CREATE_STRUCTURE:
                    this->SwitchMenuState(MenuStates::SELECT_STRUCTURE);
                    break;        
                case MenuStates::CREATE_WORLD:
                    this->SwitchMenuState(MenuStates::SELECT_WORLD);
                    break;           
                case MenuStates::CREATE_PLAYER:
                    this->SwitchMenuState(MenuStates::SELECT_PLAYER);
                    break;      
            }





        }
    }

}

void MenuScene::EnterWorld(){
    this->client->SetPlayMode(this->play_mode);
    Game::LoadScene("WorldScene");
}

void MenuScene::ClearFoundPlayersList(){
    found_players.clear();

    // deletes buttons
    for(int i = 0; i < found_players_buttons.size(); i++){
        DeleteObject(found_players_buttons[i]->GetThisObject());
    }

    found_players_buttons.clear();
}

void MenuScene::ClearFoundStructuresList(){
    found_structures.clear();

    // deletes buttons
    for(int i = 0; i < found_structures_buttons.size(); i++){
        DeleteObject(found_structures_buttons[i]->GetThisObject());
    }

    found_structures_buttons.clear();
}

void MenuScene::ClearFoundWorldsList(){
    found_worlds.clear();

    // deletes buttons
    for(int i = 0; i < found_worlds_buttons.size(); i++){
        DeleteObject(found_worlds_buttons[i]->GetThisObject());
    }

    found_worlds_buttons.clear();
}

void MenuScene::SwitchMenuState(MenuStates new_state){

    rect_array->SetActive(false);

    switch(new_state){

        case SELECT_PLAY_MODE: {
            rect_array->ClearUIRects();
            rect_array->AddUIRect(offline_button);
            rect_array->AddUIRect(join_button);
            rect_array->AddUIRect(host_button);
            rect_array->AddUIRect(structure_button);
            break;
        }

        case ENTER_JOIN_IP: {
            
            rect_array->ClearUIRects();
            rect_array->AddUIRect(join_ip_input);
            rect_array->AddUIRect(join_port_input);
            rect_array->AddUIRect(join_ip_button);
            break;
        }

        case SELECT_PLAYER : {

            rect_array->ClearUIRects();
            ClearFoundPlayersList();

            rect_array->AddUIRect(new_player_button);

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

                rect_array->AddUIRect(button);
                found_players_buttons.push_back(button);
            }

            break;
        }
        case CREATE_PLAYER : {
            rect_array->ClearUIRects();
            rect_array->AddUIRect(create_player_input);
            rect_array->AddUIRect(create_player_button);
            break;
        }
        case SELECT_WORLD : {

            rect_array->ClearUIRects();
            ClearFoundWorldsList();

            rect_array->AddUIRect(new_world_button);

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

                rect_array->AddUIRect(button);
                found_worlds_buttons.push_back(button);
            }

            break;
        }
        case CREATE_WORLD : {
            rect_array->ClearUIRects();
            rect_array->AddUIRect(create_world_input);
            rect_array->AddUIRect(create_world_button);
            break;
        }
    
        case SELECT_STRUCTURE : {

            rect_array->ClearUIRects();
            ClearFoundStructuresList();

            rect_array->AddUIRect(new_structure_button);

            // add a button for each player

            found_structures = Serilizer::LoadStructureList();
            for(int i = 0; i < found_structures.size(); i++){

                UIButton* button = AddObject<Object>()->AddComponent<UIButton>();
                button->SetString(found_structures[i].name);

                // join the world
                button->SetOnClickCallback([this, i](){
                    
                    client->SetCurrentWorld(this->found_structures[i]);
                    this->client->SetPlayMode(PlayMode::OFFLINE);

                    Core::LoadScene("EditorScene");
                    
                });

                rect_array->AddUIRect(button);
                found_structures_buttons.push_back(button);
            }

            break;
        }

        case CREATE_STRUCTURE : {
            rect_array->ClearUIRects();
            rect_array->AddUIRect(create_structure_input);
            rect_array->AddUIRect(create_structure_button);
            break;
        }
    
    
    }
    
    rect_array->SetActive(true);


    this->menu_state = new_state;
}