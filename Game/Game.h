#include "../Amber/Framework.h"
#include "Networking/GameServer.h"
#include "Networking/GameClient.h"
#include "WorldScene.h"

class Game : public Core{

    public:
        Game(bool is_server = false) : Core(1400, 800, 700, 400, "My Game")
        {
            if(is_server){
               server.Run(6868); 
            }
        }

        ~Game() override{ 
            server.Close();
        }

        void Start() override{

            // Textures ------------------------------------------------------------------

            AssetManager::CreateTexture("cursor_breaking", "breaking.png");
            AssetManager::CreateTexture("player", "player.png");
            AssetManager::CreateTexture("furnace", "furnace.png");

            AssetManager::CreateTexture("slot", "slot_sprite.png");
            AssetManager::CreateTexture("inventory_sprites", "inventory_sprites.png");

            AssetManager::CreateTexture("tiles", "tiles.png");
            AssetManager::CreateTexture("utility_tiles", "utility_objects.png");
            AssetManager::CreateTexture("foreground_tiles", "sprite_overlays.png");       
            AssetManager::CreateTexture("background_tiles", "tiles_background.png");

            AssetManager::CreateTexture("tree", "tree.png");
            AssetManager::CreateTexture("background", "background_idea.png");

            AssetManager::CreateTexture("fly", "fly.png");
            AssetManager::CreateTexture("noodle_head", "noodle_head.png");

            // Animations -----------------------------------------------------------------

            AssetManager::CreateAnimationSet("fly", { Animation("fly", 4, 0, 2, 25)}, {"default"});

            AssetManager::CreateAnimationSet("player", {Animation("player", 16, 0, 2, 200), // Idle
                                                        Animation("player", 16, 2, 6, 100), // Run
                                                        Animation("player", 16, 6, 7, 100), // FallUp
                                                        Animation("player", 16, 7, 8, 100),  // FallDown
                                                        Animation("player", 16, 8, 9, 100)  // OnWall
                                                        }, {"idle", "run", "fallup", "falldown", "onwall"});
            // Sounds ---------------------------------------------------------------------

            AssetManager::CreateSound("hit", "Sounds/FX/hit.wav");
            AssetManager::CreateSound("break", "Sounds/FX/break.wav");
            AssetManager::CreateSound("noisy_blip", "Sounds/FX/noisy_blip.wav");

            // Scenes ---------------------------------------------------------------------

            AssetManager::CreateScene<WorldScene>("WorldScene");

            LoadScene("WorldScene");

        }

        GameServer server;
};   