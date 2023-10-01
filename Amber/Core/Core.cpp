#include "Core.h"

Core::Core(int window_width, int window_height, std::string window_title):
    Core::Core(window_width, window_height, window_width, window_height, window_title){}


Core::Core(int window_width, int window_height, int display_width, int display_height, std::string window_title):
    window_height(window_width),
    window_width(window_height),
    display_width(display_width),
    display_height(display_height),
    window(sf::VideoMode(window_width, window_height), window_title),
    asset_manager(this),
    render_manager(this),
    current_scene(nullptr)
{
    if(!sf::Shader::isAvailable()){
        std::cout << "ERROR : Shaders are not supported, !sf::Shader::isAvailable\n";
    } 
}


void Core::Run(){

    this->Start();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            this->CatchEvent(event);
            current_scene->CatchEvent(event);
        }

        window.clear();

        sf::Time dt = delta_clock.restart();
        Time::SetDeltaTime(dt.asSeconds());

        this->Update();

        if(current_scene != nullptr){
            current_scene->InternalUpdate();
            render_manager.Render(window, current_scene);
        }
        else{
            std::cout << "ERROR: No scene is selected, please load using Core::LoadScene()\n";
            return;
        }
        window.display();

        Time::Increment();
    }

}

AssetManager* Core::GetAssetManager(){
    return &asset_manager;
}

void Core::LoadScene(const char* label){
    
    Scene* scene = asset_manager.GetScene(label);
    
    // empty previous scene
    if(current_scene != nullptr){
        current_scene->ClearObjects();
    }

    if(scene != nullptr){
        current_scene = scene;
        current_scene->Start();
    }
}


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

sf::RenderWindow* Core::GetWindow(){
    return &window;
}
