#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Globals.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "Scene.h"
#include "../Utility/Time.h"

class Core{

    /*
        Core:

        the entry point, acts as the brains of the program.
        By inheriting Core we can override and utalise its methods, specifically Start(), Update(), PollEvents(),
    
    */

    public:      
        Core(int window_width = 800, int window_height = 800, std::string window_title = "Untitled Project");
        Core(int window_width, int window_height, int display_width, int display_height, std::string window_title = "Untitled Project");
        void Run();

        virtual void Start(){}
        virtual void Update(){}
        virtual void CatchEvent(sf::Event event){}

        AssetManager* GetAssetManager();
        /*
            Loads a new scene
            @param label The name of the scene (saved in AssetManager)
        */
        void LoadScene(const char* label);

        void SetWindowSize(sf::Vector2i window_size);
        void SetWindowSize(int width, int height);
        sf::Vector2i GetWindowSize();

        void SetDisplaySize(sf::Vector2i display_size);
        void SetDisplaySize(int width, int height);
        sf::Vector2i GetDisplaySize();      

        sf::RenderWindow* GetWindow();


    private:

        sf::Clock delta_clock;

        Scene* current_scene;
        
        AssetManager asset_manager;
        RenderManager render_manager;
        
        sf::RenderWindow window;

        int display_width; // size of the screen before stretching to window size
        int display_height; 
        
        int window_width; // actual size of our window
        int window_height;

};