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
        the entry point, acts as the brains of the program.
        By inheriting Core we can override and utalise its methods, specifically Start(), Update(), PollEvents(),
    
    */

    public:      
        Core(int _window_width = 800, int _window_height = 800, std::string window_title = "Untitled Project");
        Core(int _window_width, int _window_height, int _display_width, int _display_height, std::string window_title = "Untitled Project");
        
        virtual ~Core(){}

        void Run();

        virtual void Start(){}
        virtual void Update(){}
        virtual void CatchEvent(sf::Event event){}

        /*
            Loads a new scene
            @param label The name of the scene (saved in AssetManager)
        */
        static void LoadScene(const char* label);

        static sf::RenderWindow* GetWindow();

        static void SetWindowSize(sf::Vector2i window_size);
        static void SetWindowSize(int width, int height);
        static sf::Vector2i GetWindowSize();
        static int GetWindowWidth(){return window_width; }
        static int GetWindowHeight(){return window_height; }

        static void SetDisplaySize(sf::Vector2i display_size);
        static void SetDisplaySize(int width, int height);
        static sf::Vector2i GetDisplaySize();   
        static int GetDisplayWidth(){return display_width; }
        static int GetDisplayHeight(){return display_height; }

        // remapping a position from 0->display_size, to 0->window_size
        static sf::Vector2i DisplayToWindowPosition(sf::Vector2i _display_position);   
        // the scaling factor to get from a display position to window position
        static sf::Vector2f GetDisplayToWindowMultiplier() { return display_to_window_multiplier; }
        static sf::Vector2f GetWindowToDisplayMultiplier() { return window_to_display_multiplier; }

        static bool DEBUG_MODE;

    private:

        
        static sf::Clock delta_clock;
        static sf::RenderWindow window;
        static Scene* current_scene;
        
        static int display_width; // size of the screen before stretching to window size
        static int display_height; 
        
        static sf::Vector2f display_to_window_multiplier; 
        static sf::Vector2f window_to_display_multiplier;
        static int window_width; // actual size of our window
        static int window_height;

};