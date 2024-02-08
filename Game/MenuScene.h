#pragma once
#include "../Amber/Framework.h"
#include "Player/Inventory.h"
#include "Serilizer.h"
#include "World/World.h"
#include "PlayMode.h"

class MenuScene : public Scene {

    public: 

        enum MenuStates{
            MAIN,

            SELECT_PLAY_MODE,

            SELECT_PLAYER,
            CREATE_PLAYER,

            SELECT_WORLD,
            CREATE_WORLD,
            GENERATING_WORLD,

            SETTINGS,
        };

        void Start() override;

        void ClearFoundPlayersList();
        void ClearFoundWorldsList();

        void EnterWorld();

        void SwitchMenuState(MenuStates new_state);

    private:
    
        MenuStates menu_state;
        PlayMode play_mode;

        GameClient* client;

        UIButton* play_button;
        UIButton* settings_button;
        UIButton* quit_button;

        UIButton* offline_button;
        UIButton* join_button;
        UIButton* host_button;
        UIRectArray* play_mode_rect_array;

        UIRectArray* main_rect_array;

        UIButton* new_player_button;
        UIRectArray* select_player_rect_array;

        std::vector<Serilizer::DataPair> found_players;
        std::vector<UIButton*> found_players_buttons;

        UIInputField* create_player_input;
        UIButton* create_player_button;
        UIRectArray* create_player_rect_array;

        UIButton* new_world_button;
        UIRectArray* select_world_rect_array;
        

        std::vector<Serilizer::DataPair> found_worlds;
        std::vector<UIButton*> found_worlds_buttons;

        UIInputField* create_world_input;
        UIButton* create_world_button;
        UIRectArray* create_world_rect_array;

};