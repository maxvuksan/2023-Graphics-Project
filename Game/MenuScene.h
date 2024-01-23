#pragma once
#include "../Amber/Framework.h"
#include "Player/Inventory.h"
#include "Serilizer.h"

class MenuScene : public Scene {

    public: 

        enum MenuStates{
            MAIN,

            SELECT_PLAYER,
            CREATE_PLAYER,
            SELECT_WORLD,
            /*
                PLAY_SOLO,
                PLAY_HOST_GAME,
                PLAY_JOIN_GAME
            */
            
            SETTINGS,
        };

        MenuStates menu_state;


        void Start(){

            Camera* camera = AddObject<Object>()->AddComponent<Camera>();
            camera->background_colour = sf::Color(42, 40, 48);
            SetActiveCamera(camera);


            // Main --------------------------------------------------------------------------

            play_button = AddUI<Object>()->AddComponent<UIButton>();
            play_button->SetString("Play");

            play_button->SetOnClickCallback([this](){

                this->SwitchMenuState(MenuStates::SELECT_PLAYER);
            });

            settings_button = AddUI<Object>()->AddComponent<UIButton>();
            settings_button->SetString("Settings");
            settings_button->SetOnClickCallback([](){
                std::cout << "settings pressed\n";
            });

            main_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
            main_rect_array->AddUIRect(play_button);
            main_rect_array->AddUIRect(settings_button);
            main_rect_array->SetElementSize(150, 50);
            main_rect_array->SetGap(UIRect::padding);
            main_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);


            // Select Player ----------------------------------------------------------------

            new_player_button = AddUI<Object>()->AddComponent<UIButton>();
            new_player_button->SetString("New Player +");
            new_player_button->SetOnClickCallback([this](){
                
                // create new player save
                this->SwitchMenuState(MenuStates::CREATE_PLAYER);
            });       

            select_player_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
            select_player_rect_array->SetElementSize(150, 50);
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

                Serilizer::CreateNewPlayer(create_player_input->GetInput());
                this->SwitchMenuState(MenuStates::SELECT_PLAYER);
            });

            create_player_rect_array = AddUI<Object>()->AddComponent<UIRectArray>();
            create_player_rect_array->SetElementSize(150, 50);
            create_player_rect_array->SetGap(UIRect::padding);
            create_player_rect_array->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
            create_player_rect_array->AddUIRect(create_player_input);
            create_player_rect_array->AddUIRect(create_player_button);




            this->SwitchMenuState(MenuStates::MAIN);
        }

        void SwitchMenuState(MenuStates new_state){

            switch(new_state){

                case MAIN : {
                    main_rect_array->SetActive(true);
                    select_player_rect_array->SetActive(false);
                    create_player_rect_array->SetActive(false);
                    break;
                }
                case SELECT_PLAYER : {

                    select_player_rect_array->SetActive(true);
                    select_player_rect_array->ClearUIRects();
                    select_player_rect_array->AddUIRect(new_player_button);

                    main_rect_array->SetActive(false);
                    create_player_rect_array->SetActive(false);
                    break;
                }
                case CREATE_PLAYER : {

                    select_player_rect_array->SetActive(false);
                    main_rect_array->SetActive(false);
                    create_player_rect_array->SetActive(true);
                    break;
                }
                case SELECT_WORLD : {
                    main_rect_array->SetActive(false);
                    select_player_rect_array->SetActive(false);
                    create_player_rect_array->SetActive(false);
                    break;
                }
            }

            this->menu_state = new_state;
        }

        void Update(){

        }

    private:

        UIButton* play_button;
        UIButton* settings_button;
        UIButton* quit_button;

        UIRectArray* main_rect_array;

        UIButton* new_player_button;
        UIRectArray* select_player_rect_array;

        UIInputField* create_player_input;
        UIButton* create_player_button;
        UIRectArray* create_player_rect_array;
};