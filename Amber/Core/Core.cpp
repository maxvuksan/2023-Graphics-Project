#include "Core.h"
#include "../Utility/Sound.h"
#include <cmath>
#include <SFML/OpenGL.hpp>

// Static Members ///////////////////////////////////////////////////

bool Core::DEBUG_MODE = false;

sf::Clock Core::delta_clock;

sf::RenderWindow Core::window;

Scene* Core::current_scene;

sf::Vector2f Core::display_to_window_multiplier;
sf::Vector2f Core::window_to_display_multiplier;

int Core::display_width; 
int Core::display_height;         
int Core::window_width; 
int Core::window_height;

/////////////////////////////////////////////////////////////////////

Core::Core(int _window_width, int _window_height, std::string window_title):
    Core::Core(_window_width, _window_height, _window_width, _window_height, window_title){}


Core::Core(int _window_width, int _window_height, int _display_width, int _display_height, std::string window_title)
{
    window.create(sf::VideoMode(_window_width, _window_height), window_title);
    window.setFramerateLimit(Globals::FRAMERATE_LIMIT);
    
    current_scene = nullptr;
    display_width = _display_width;
    display_height = _display_height,
    window_width = _window_width;
    window_height = _window_height;

    display_to_window_multiplier.x = window_width / (float)display_width;
    display_to_window_multiplier.y = window_height / (float)display_height;

    window_to_display_multiplier.x = display_width / (float)window_width;
    window_to_display_multiplier.y = display_height / (float)window_height;

    if(!sf::Shader::isAvailable()){
        std::cout << "ERROR : Shaders are not supported, !sf::Shader::isAvailable\n";
    } 
}

void Core::Run(){

    AssetManager::Construct();
    RenderManager::Construct();
    Sound::Construct();

    this->Start();

    while (window.isOpen())
    {

        if(current_scene == nullptr){
            std::cout << "ERROR : No Scene is selected, please load with Core::LoadScene()";
            return;
        }
        
        window.clear();

        // catch events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }

            // updating window and display dimensions
            if (event.type == sf::Event::Resized)
            {
                window_width = event.size.width;
                window_height = event.size.height;

                display_width = round((float)window_width * window_to_display_multiplier.x);
                display_height = round((float)window_height * window_to_display_multiplier.y);

                std::cout << display_width << "\n";
                std::cout << display_height << "\n";

                window.setSize(sf::Vector2u(event.size.width, event.size.height));
                glViewport(0, 0, window_width, window_height);
            }

            this->CatchEvent(event);
            current_scene->InternalCatchEvent(event);
        }

        // calculating delta time...
        sf::Time dt = delta_clock.restart();
        Time::SetDeltaTime(dt.asSeconds());


        // performing updates
        this->Update();
        current_scene->InternalUpdate();

        // calculate mouse positions
        Mouse::SetWindowPosition(sf::Mouse::getPosition(window));
        Mouse::UpdateDisplayPosition();

        RenderManager::Render(window, current_scene);

        window.display();
        Time::Increment();
    }
    
    current_scene->ClearAll();
    AssetManager::Destruct();
    RenderManager::Destruct();
}

void Core::LoadScene(const char* label){
    
    
    // empty previous scene
    if(current_scene != nullptr){
        current_scene->ClearAll();
    }
    
    Scene* scene = AssetManager::GetScene(label);

    Scene::SetActiveCamera(nullptr);


    if(scene != nullptr){
        current_scene = scene;
        current_scene->Start();
    }

}

sf::RenderWindow* Core::GetWindow(){
    return &window;
}

// Static Methods /////////////////////////////////////////////////////////

void Core::SetWindowSize(sf::Vector2i window_size){
    window_width = window_size.x;
    window_height = window_size.y;
}

void Core::SetWindowSize(int width, int height){
    window_width = width;
    window_height = height;
}

sf::Vector2i Core::GetWindowSize(){
    return sf::Vector2i(window_width, window_height);
}

void Core::SetDisplaySize(sf::Vector2i display_size){
    display_width = display_size.x;
    display_height = display_size.y;
}

void Core::SetDisplaySize(int width, int height){
    display_width = width;
    display_height = height;
}

sf::Vector2i Core::GetDisplaySize(){
    return sf::Vector2i(display_width, display_height);
}

sf::Vector2i Core::DisplayToWindowPosition(sf::Vector2i _display_position){
    return sf::Vector2i(_display_position.x * display_to_window_multiplier.x, _display_position.y * display_to_window_multiplier.y);
}

