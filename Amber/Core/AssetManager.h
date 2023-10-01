#pragma once
#include <unordered_map>
#include <iostream>

#include "Scene.h"

/*
    AssetManager: 

    responsible for storing resusable assets, preventing duplication 

    this could include

    Animations
    Scenes
    Textures
    etc...

*/
class Core;
class AssetManager {

    public:
        AssetManager(Core* core);
        ~AssetManager();

        // only fragment shaders are supported
        sf::Shader* CreateShader(const char* label, const std::string fragment_file_location);
        sf::Shader* GetShader(const char* label);
        
        sf::Texture* CreateTexture(const char* label, const sf::RenderTexture& render_texture, bool repeat = false);
        sf::Texture* CreateTexture(const char* label, const char* file_location, bool repeat = false);
        sf::Texture* GetTexture(const char* label);
        /*
            Creates a scene of the specified type T (allowing only classes which inherit Scene)
        */
        template <typename T>
        Scene* CreateScene(const char* label) {

            Scene* new_scene = new T;
            new_scene->LinkCore(core);
            scenes.insert(std::make_pair(label, new_scene));

            return new_scene;
        }

        /*
            @returns A pointer to a desired scene
            @param label The name the scene is saved as  
        */
        Scene* GetScene(const char* label); 

    protected:
        Core* core;

        std::unordered_map<const char*, sf::Texture> textures; 
        std::unordered_map<const char*, Scene*> scenes; /*scenes are stored as pointers to allow for inherited scenes*/
        std::unordered_map<const char*, sf::Shader*> shaders;
};