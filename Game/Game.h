#pragma once

#include "../Amber/Framework.h"
#include "Networking/GameServer.h"
#include "Networking/GameClient.h"
#include "WorldScene.h"
#include "MenuScene.h"

#include "GameUI/SlotSet.h"

class Game : public Core{

    public:
        Game(bool _is_server = false) : Core(800, 400, 800, 400, "My Game")
        {
            this->is_server = _is_server;

            if(_is_server){
               server.Run(6868); 
            }
        }

        ~Game() override{ 
            server.Close();
        }

        void Start() override{

            // Fonts ---------------------------------------------------------------------

            AssetManager::CreateFontObj("m3x6", "m3x6.ttf");

            // Textures ------------------------------------------------------------------

            AssetManager::CreateTexture("player", "Images/player.png");
            AssetManager::CreateTexture("furnace", "Images/furnace.png");

            AssetManager::CreateTexture("slot", "Images/slot_sprite.png");
            AssetManager::CreateTexture("inventory_sprites", "Images/inventory_sprites.png");
            AssetManager::CreateTexture("map_icon", "Images/map_icon.png");
            AssetManager::CreateTexture("tool_slash", "Images/tool_slash.png");

            AssetManager::CreateTexture("tiles", "Images/tiles.png");
            AssetManager::CreateTexture("utility_tiles", "Images/utility_objects.png");
            AssetManager::CreateTexture("foreground_tiles", "Images/sprite_overlays.png");       
            AssetManager::CreateTexture("background_tiles", "Images/tiles_background.png");
            AssetManager::CreateTexture("foliage", "Images/foliage.png");
            AssetManager::CreateTexture("water", "Images/water.png");

            AssetManager::CreateTexture("tree", "Images/tree.png");
            AssetManager::CreateTexture("background", "Images/background_idea.png");

            AssetManager::CreateTexture("fly", "Images/fly.png");
            AssetManager::CreateTexture("noodle_head", "Images/noodle_head.png");

            AssetManager::CreateTexture("cursor", "Images/crosshair.png");
            AssetManager::CreateTexture("cursor_breaking", "Images/breaking.png");

            // Animations -----------------------------------------------------------------

            AssetManager::CreateAnimationSet("fly", { Animation("fly", 4, 0, 2, 25)}, {"default"});

            AssetManager::CreateAnimationSet("player", {Animation("player", 16, 0, 2, 200), // Idle
                                                        Animation("player", 16, 2, 6, 100), // Run
                                                        Animation("player", 16, 6, 7, 100), // FallUp
                                                        Animation("player", 16, 7, 8, 100),  // FallDown
                                                        Animation("player", 16, 8, 9, 100)  // OnWall
                                                        }, {"idle", "run", "fallup", "falldown", "onwall"});

            AssetManager::CreateAnimationSet("tool_slash", { Animation("tool_slash", 28, 0, 4, {50, 100, 50, 50}, false)}, {"default"});
            // Sounds ---------------------------------------------------------------------

            AssetManager::CreateSound("hit", "Sounds/FX/hit.wav");
            AssetManager::CreateSound("break", "Sounds/FX/break.wav");
            AssetManager::CreateSound("noisy_blip", "Sounds/FX/noisy_blip.wav");
            AssetManager::CreateSound("player_damage", "Sounds/FX/player_damage.wav");

            AssetManager::CreateSound("item_metal", "Sounds/FX/Inventory/item_Metal.wav");
            AssetManager::CreateSound("item_rubble", "Sounds/FX/Inventory/item_Rubble.wav");
            AssetManager::CreateSound("item_soft", "Sounds/FX/Inventory/item_Soft.wav");
      

            // Scenes ---------------------------------------------------------------------

            AssetManager::CreateScene<MenuScene>("MenuScene");

            // config ui button
            UIButton::GetText().setFont(*AssetManager::GetFont("m3x6"));
            UIButton::GetText().setString("Play");
            UIButton::GetText().setCharacterSize(32);

           // Scene* world_scene = AssetManager::CreateScene<WorldScene>("WorldScene");
           // world_scene->SetClient(&client);

            if(is_server){
           //     world_scene->SetServer(&server);
            }

            //client.Connect("127.0.0.1", 6868);

            // Static Constructions --------------------------------------------------

            Slot::Construct();
            SlotSet::Construct();

            // ----------------------------------------------------------------------

            LoadScene("MenuScene");
            //LoadScene("WorldScene");
        }
        
        // dynamically casts a scenes generic client pointer to a GameClient pointer
        static GameClient* GetGameClientFromScene(Scene* scene){
            return dynamic_cast<GameClient*>(scene->GetClient());
        }

        static GameServer* GetGameServerFromScene(Scene* scene){
            return dynamic_cast<GameServer*>(scene->GetServer());
        }

        bool is_server;
        GameClient client;
        GameServer server;
};   