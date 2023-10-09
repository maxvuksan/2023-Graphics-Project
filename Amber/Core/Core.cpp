#include "Core.h"

Core::Core(int window_width, int window_height, std::string window_title):
    Core::Core(window_width, window_height, window_width, window_height, window_title){}


Core::Core(int window_width, int window_height, int display_width, int display_height, std::string window_title):
    window_height(window_width),
    window_width(window_height),
    display_width(display_width),
    display_height(display_height),
    window(sf::VideoMode(window_width, window_height), window_title),
    current_scene(nullptr)
{


    if(!sf::Shader::isAvailable()){
        std::cout << "ERROR : Shaders are not supported, !sf::Shader::isAvailable\n";
    } 
}

void Core::Run(){

    AssetManager::Construct(this);
    RenderManager::Construct(this);

    this->Start();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            this->CatchEvent(event);
            current_scene->InternalCatchEvent(event);
        }

        window.clear();

        sf::Time dt = delta_clock.restart();
        Time::SetDeltaTime(dt.asSeconds());

        this->Update();

        if(current_scene != nullptr){
            current_scene->InternalUpdate();

            Mouse::SetWindowPosition(sf::Mouse::getPosition(window));
            RenderManager::Render(window, current_scene);
        }
        else{
            std::cout << "ERROR: No scene is selected, please load using Core::LoadScene()\n";
            return;
        }
        if(Globals::DEBUG_MODE){
            sf::Text fps_text;
            fps_text.setString("FPS " + std::to_string(Time::FPS())); 
            fps_text.setPosition(sf::Vector2f(10,10));
            window.draw(fps_text);
        }
        window.display();

        Time::Increment();
    }

    AssetManager::Destruct();
    RenderManager::Destruct();
}

void Core::LoadScene(const char* label){
    
    Scene* scene = AssetManager::GetScene(label);
    
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
