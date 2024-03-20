#pragma once
#include "../Amber/Framework.h"
#include "Player/Inventory.h"
#include "Serilizer.h"
#include "World/World.h"
#include "PlayMode.h"

class MenuScene : public Scene {

    public: 

        enum MenuStates{

            SELECT_PLAY_MODE,
            ENTER_JOIN_IP,

            SELECT_PLAYER,
            CREATE_PLAYER,

            SELECT_WORLD,
            CREATE_WORLD,

            SELECT_STRUCTURE,
            CREATE_STRUCTURE,
        };

        void Start() override;

        void CatchEvent(sf::Event) override;

        void ClearFoundPlayersList();
        void ClearFoundWorldsList();
        void ClearFoundStructuresList();

        void EnterWorld();

        void SwitchMenuState(MenuStates new_state);

    private:
    
        MenuStates menu_state;
        MenuStates previous_menu_state;

        PlayMode play_mode;

        GameClient* client;

        UIButton* offline_button;
        UIButton* join_button;
        UIButton* host_button;
        UIButton* structure_button;

        UIInputField* join_ip_input;
        UIInputField* join_port_input;
        UIButton* join_ip_button;

        UIButton* new_player_button;

        std::vector<Serilizer::DataPair> found_players;
        std::vector<UIButton*> found_players_buttons;

        UIInputField* create_player_input;
        UIButton* create_player_button;

        std::vector<Serilizer::DataPair> found_worlds;
        std::vector<UIButton*> found_worlds_buttons;
        UIButton* new_world_button;
        
        UIInputField* create_world_input;
        UIButton* create_world_button;

        std::vector<Serilizer::DataPair> found_structures;
        std::vector<UIButton*> found_structures_buttons;
        UIButton* new_structure_button;
        
        UIInputField* create_structure_input;
        UIButton* create_structure_button;

        UIRectArray* rect_array;

};