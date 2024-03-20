#pragma once

#include "../Amber/Framework.h"
#include "Networking/GameServer.h"
#include "Networking/GameClient.h"
#include "WorldScene.h"
#include "MenuScene.h"
#include "EditorScene.h"

#include "GameUI/SlotSet.h"

#include "../steam/isteamfriends.h"
#include "../steam/steam_api.h"

class Game : public Core{

    public:
        Game() : Core(1800, 900, 600, 300, "Blocky block")
        {
            if(!SteamAPI_Init()){
                std::cout << "ERROR: Failed to call SteamAPI_Init, Game::Game()\n";
            }
            // when accessed through steam we should include the function...
            // SteamAPI_RestartAppIfNecessary()
        }

        ~Game() override{ 
            server.Close();

            // cleanup any resources associated with steam
            SteamAPI_Shutdown();
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
            AssetManager::CreateTexture("structures", "Images/structures.png");
            AssetManager::CreateTexture("water", "Images/water.png");

            AssetManager::CreateTexture("tree", "Images/tree.png");
            AssetManager::CreateTexture("background", "Images/background_idea.png");

            AssetManager::CreateTexture("fly", "Images/fly.png");
            AssetManager::CreateTexture("noodle_head", "Images/noodle_head.png");

            AssetManager::CreateTexture("cursor", "Images/crosshair.png");
            AssetManager::CreateTexture("cursor_breaking", "Images/breaking.png");

            AssetManager::CreateTexture("heavy", "Images/heavy.png");

            // Animations -----------------------------------------------------------------

            AssetManager::CreateAnimationSet("fly", { Animation("fly", 4, 0, 2, 25)}, {"default"});

            AssetManager::CreateAnimationSet("player", {Animation("player", 20, 0, 2, 350), // Idle
                                                        Animation("player", 20, 2, 10, {105,45,105,45,105,45,105,45}), // Run
                                                        Animation("player", 20, 10, 11, 100), // FallUp
                                                        Animation("player", 20, 11, 12, 100),  // FallDown
                                                        Animation("player", 20, 11, 12, 100)  // OnWall
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

            Scene* menu_scene = AssetManager::CreateScene<MenuScene>("MenuScene");
            menu_scene->SetClient(&client);
            menu_scene->SetServer(&server);

            Scene* world_scene = AssetManager::CreateScene<WorldScene>("WorldScene");
            world_scene->SetClient(&client);
            world_scene->SetServer(&server);

            Scene* editor_scene = AssetManager::CreateScene<EditorScene>("EditorScene");
            editor_scene->SetClient(&client);
            editor_scene->SetServer(&server);

            // config ui button
            UIButton::GetText().setFont(*AssetManager::GetFont("m3x6"));
            UIButton::GetText().setString("Play");
            UIButton::GetText().setCharacterSize(32);


            // Static Constructions --------------------------------------------------

            Slot::Construct();
            SlotSet::Construct();

            // ----------------------------------------------------------------------

            LoadScene("MenuScene");
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